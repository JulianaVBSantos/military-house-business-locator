#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lojas.h"
#include "cJSON.h"

Loja lojas[MAX_LOJAS];
int totalLojas = 0;

void paraMinusculo(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

static void copiarStringSeguro(char *destino, const char *origem, size_t tamanho) { // funcao interna para copiar os dados e evitar crash
    if (origem != NULL) {
        strncpy(destino, origem, tamanho - 1);
        destino[tamanho - 1] = '\0';
    } else {
        strncpy(destino, "N/A", tamanho - 1);
        destino[tamanho - 1] = '\0';
    }
}

void carregarLojas(const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END); 
    long tamanho = ftell(fp); // ftell retorna a posição atual em bytes
    rewind(fp);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (!conteudo) {
        printf("Erro de memória.\n");
        fclose(fp);
        return;
    }

    size_t lido = fread(conteudo, 1, tamanho, fp);
if (lido == 0) {
    printf("Erro ao ler o arquivo ou arquivo vazio.\n");
    free(conteudo);
    fclose(fp);
    return;
}

    conteudo[tamanho] = '\0';
    fclose(fp);

    cJSON *json = cJSON_Parse(conteudo);
    if (json == NULL) {
        printf("Erro ao interpretar JSON.\n");
        free(conteudo);
        return;
    }

    if (!cJSON_IsArray(json)) {
        printf("Formato de JSON inválido (esperado array).\n");
        cJSON_Delete(json);
        free(conteudo);
        return;
    }

    int tamanhoArray = cJSON_GetArraySize(json);
    totalLojas = 0;

    for (int i = 0; i < tamanhoArray && i < MAX_LOJAS; i++) {
        cJSON *lojaJSON = cJSON_GetArrayItem(json, i);

        if (!cJSON_IsObject(lojaJSON)) continue;

        cJSON *nome = cJSON_GetObjectItem(lojaJSON, "nome");
        cJSON *contato = cJSON_GetObjectItem(lojaJSON, "contato");
        cJSON *vendedora = cJSON_GetObjectItem(lojaJSON, "vendedora");
        cJSON *endereco = cJSON_GetObjectItem(lojaJSON, "endereco");
        cJSON *cidade = cJSON_GetObjectItem(lojaJSON, "cidade");
        cJSON *estado = cJSON_GetObjectItem(lojaJSON, "estado");
        cJSON *cepmg = cJSON_GetObjectItem(lojaJSON, "cepmg");
        cJSON *cep = cJSON_GetObjectItem(lojaJSON, "cep");

        copiarStringSeguro(lojas[i].nome, cJSON_IsString(nome) ? nome->valuestring : NULL, sizeof(lojas[i].nome));
        copiarStringSeguro(lojas[i].contato, cJSON_IsString(contato) ? contato->valuestring : NULL, sizeof(lojas[i].contato));
        copiarStringSeguro(lojas[i].vendedora, cJSON_IsString(vendedora) ? vendedora->valuestring : NULL, sizeof(lojas[i].vendedora));
        copiarStringSeguro(lojas[i].endereco, cJSON_IsString(endereco) ? endereco->valuestring : NULL, sizeof(lojas[i].endereco));
        copiarStringSeguro(lojas[i].cidade, cJSON_IsString(cidade) ? cidade->valuestring : NULL, sizeof(lojas[i].cidade));
        copiarStringSeguro(lojas[i].estado, cJSON_IsString(estado) ? estado->valuestring : NULL, sizeof(lojas[i].estado));
        copiarStringSeguro(lojas[i].cepmg, cJSON_IsString(cepmg) ? cepmg->valuestring : NULL, sizeof(lojas[i].cepmg));
        copiarStringSeguro(lojas[i].cep, cJSON_IsString(cep) ? cep->valuestring : NULL, sizeof(lojas[i].cep));

        totalLojas++;
    }

    cJSON_Delete(json);
    free(conteudo);
}

void salvarLojas(const char *arquivo) {
    cJSON *arrayLojas = cJSON_CreateArray();

    for (int i = 0; i < totalLojas; i++) {
        cJSON *lojaJSON = cJSON_CreateObject();

        cJSON_AddStringToObject(lojaJSON, "nome", lojas[i].nome);
        cJSON_AddStringToObject(lojaJSON, "contato", lojas[i].contato);
        cJSON_AddStringToObject(lojaJSON, "vendedora", lojas[i].vendedora);
        cJSON_AddStringToObject(lojaJSON, "endereco", lojas[i].endereco);
        cJSON_AddStringToObject(lojaJSON, "cidade", lojas[i].cidade);
        cJSON_AddStringToObject(lojaJSON, "estado", lojas[i].estado);
        cJSON_AddStringToObject(lojaJSON, "cepmg", lojas[i].cepmg);
        cJSON_AddStringToObject(lojaJSON, "cep", lojas[i].cep);

        cJSON_AddItemToArray(arrayLojas, lojaJSON);
    }

    char *stringJSON = cJSON_Print(arrayLojas);

    FILE *fp = fopen(arquivo, "w");
    if (fp == NULL) {
        printf("Erro ao salvar arquivo.\n");
        cJSON_Delete(arrayLojas);
        return;
    }

    fprintf(fp, "%s", stringJSON);
    fclose(fp);

    free(stringJSON);
    cJSON_Delete(arrayLojas);

    printf("Dados salvos com sucesso!\n");
}