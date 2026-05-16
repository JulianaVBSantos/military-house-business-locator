#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lojas.h"
#include "cJSON.h"

void adicionarLoja()
{

    if (totalLojas >= MAX_LOJAS)
    {
        printf("Limite de lojas atingido.\n");
        return;
    }

    Loja novaLoja;

    printf("\n=== ADICIONAR LOJA ===\n");

    lerCampoObrigatorio(novaLoja.nome, sizeof(novaLoja.nome), "Nome da loja: ");

    lerCampoObrigatorio(novaLoja.contato, sizeof(novaLoja.contato), "Contato: ");

    lerCampoObrigatorio(novaLoja.vendedora, sizeof(novaLoja.vendedora), "Vendedora: ");

    lerCampoObrigatorio(novaLoja.endereco, sizeof(novaLoja.endereco), "Endereço: ");

    lerCampoObrigatorio(novaLoja.cidade, sizeof(novaLoja.cidade), "Cidade: ");

    lerCampoObrigatorio(novaLoja.estado, sizeof(novaLoja.estado), "Estado: ");

    lerCampoObrigatorio(novaLoja.cepmg, sizeof(novaLoja.cepmg), "CEPMG: ");

    lerCampoObrigatorio(novaLoja.cep, sizeof(novaLoja.cep), "CEP: ");

    printf("Latitude: ");
    if (scanf("%lf", &novaLoja.lat) != 1)
    {
        printf("Latitude inválida.\n");
        getchar();
        return;
    }
    getchar();

    printf("Longitude: ");
    printf("Longitude: ");
    if (scanf("%lf", &novaLoja.lng) != 1)
    {
        printf("Longitude inválida.\n");
        getchar();
        return;
    }
    getchar();

    char confirmacao;

    printf("\nDeseja confirmar a adição desta loja? (s/n): ");

    scanf(" %c", &confirmacao);
    getchar();

    confirmacao = tolower(confirmacao);

    if (confirmacao != 's')
    {
        printf("Adição cancelada.\n");
        return;
    }

    lojas[totalLojas] = novaLoja;
    totalLojas++;

    printf("\nLoja adicionada com sucesso!\n");
}

void removerLoja()
{

    char nomeBusca[100];
    int encontrada;

    while (1)
    {

        encontrada = -1;

        printf("\n=== REMOVER LOJA ===\n");

        printf("Digite o nome da loja que deseja remover\n");
        printf("(ou digite 0 para cancelar): ");

        fgets(nomeBusca, sizeof(nomeBusca), stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

        if (strcmp(nomeBusca, "0") == 0)
        {
            printf("Operação cancelada.\n");
            return;
        }

        if (strlen(nomeBusca) == 0)
        {
            printf("Campo obrigatório.\n");
            continue;
        }

        for (int i = 0; i < totalLojas; i++)
        {

            char nomeTemp[100];
            char buscaTemp[100];

            strncpy(nomeTemp, lojas[i].nome, sizeof(nomeTemp) - 1);
            strncpy(buscaTemp, nomeBusca, sizeof(buscaTemp) - 1);

            paraMinusculo(nomeTemp);
            paraMinusculo(buscaTemp);

            removerAcentos(nomeTemp);
            removerAcentos(buscaTemp);

            if (strcmp(nomeTemp, buscaTemp) == 0)
            {
                encontrada = i;
                break;
            }
        }

        if (encontrada == -1)
        {
            printf("Loja não encontrada. Tente novamente.\n");
            continue;
        }

        char confirmacao;

        printf("\nTem certeza que deseja remover a loja '%s'? (s/n): ",
               lojas[encontrada].nome);

        scanf(" %c", &confirmacao);
        getchar();

        confirmacao = tolower(confirmacao);

        if (confirmacao != 's')
        {
            printf("Remoção cancelada.\n");
            return;
        }

        for (int i = encontrada; i < totalLojas - 1; i++)
        {
            lojas[i] = lojas[i + 1];
        }

        totalLojas--;

        printf("Loja removida com sucesso!\n");
        break;
    }
}
void editarLoja()
{

    char nomeBusca[100];
    int encontrada = -1;

    printf("\n=== EDITAR LOJA ===\n");

    printf("Digite o nome da loja que deseja editar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < totalLojas; i++)
    {

        char nomeTemp[100];
        char buscaTemp[100];

        strncpy(nomeTemp, lojas[i].nome, sizeof(nomeTemp) - 1);
        strncpy(buscaTemp, nomeBusca, sizeof(buscaTemp) - 1);

        paraMinusculo(nomeTemp);
        paraMinusculo(buscaTemp);

        removerAcentos(nomeTemp);
        removerAcentos(buscaTemp);

        if (strcmp(nomeTemp, buscaTemp) == 0)
        {
            encontrada = i;
            break;
        }
    }

    if (encontrada == -1)
    {
        printf("Loja não encontrada.\n");
        return;
    }

    Loja lojaEditada;

    lojaEditada = lojas[encontrada]; // copia os dados atuais da loja

    printf("\n=== NOVOS DADOS ===\n");

    lerCampoObrigatorio(
        lojaEditada.nome,
        sizeof(lojaEditada.nome),
        "Nome da loja: ");

    lerCampoObrigatorio(
        lojaEditada.contato,
        sizeof(lojaEditada.contato),
        "Contato: ");

    lerCampoObrigatorio(
        lojaEditada.vendedora,
        sizeof(lojaEditada.vendedora),
        "Vendedora: ");

    lerCampoObrigatorio(
        lojaEditada.endereco,
        sizeof(lojaEditada.endereco),
        "Endereço: ");

    lerCampoObrigatorio(
        lojaEditada.cidade,
        sizeof(lojaEditada.cidade),
        "Cidade: ");

    lerCampoObrigatorio(
        lojaEditada.estado,
        sizeof(lojaEditada.estado),
        "Estado: ");

    lerCampoObrigatorio(
        lojaEditada.cepmg,
        sizeof(lojaEditada.cepmg),
        "CEPMG: ");

    lerCampoObrigatorio(
        lojaEditada.cep,
        sizeof(lojaEditada.cep),
        "CEP: ");

    printf("Latitude: ");
    scanf("%lf", &lojaEditada.lat);
    getchar();

    printf("Longitude: ");
    scanf("%lf", &lojaEditada.lng);
    getchar();

    printf("\nDeseja confirmar a edição desta loja? (s/n): ");

    char confirmacao;

    scanf(" %c", &confirmacao);
    getchar();

    confirmacao = tolower(confirmacao);

    if (confirmacao != 's')
    {
        printf("Edição cancelada.\n");
        return;
    }

    lojas[encontrada] = lojaEditada; // salva somente se confirmar

    printf("\nLoja editada com sucesso!\n");
}

void buscarPorNome(const char *nomeBusca)
{ // RF02
    char busca[100];
    strncpy(busca, nomeBusca, sizeof(busca) - 1);
    paraMinusculo(busca);
    removerAcentos(busca);

    int encontrada = 0;

    for (int i = 0; i < totalLojas; i++)
    {
        char nomeTemp[100];
        strncpy(nomeTemp, lojas[i].nome, sizeof(nomeTemp) - 1);
        paraMinusculo(nomeTemp);
        removerAcentos(nomeTemp);

        if (strstr(nomeTemp, busca) != NULL)
        {
            printf("\n=== LOJA ENCONTRADA ===\n");
            printf("Nome: %s\n", lojas[i].nome);
            printf("Vendedora: %s\n", lojas[i].vendedora);
            printf("Contato: %s\n", lojas[i].contato);
            printf("Endereço: %s\n", lojas[i].endereco);
            printf("Cidade: %s\n", lojas[i].cidade);
            printf("Estado: %s\n", lojas[i].estado);
            printf("CEPMG: %s\n", lojas[i].cepmg);
            printf("CEP: %s\n\n", lojas[i].cep);

            encontrada = 1;
        }
    }

    if (!encontrada)
    {
        printf("Nenhuma loja encontrada.\n");
    }
}

void buscarPorCEPMG(const char *cepmgBusca)
{ // RF03
    char busca[100];
    strncpy(busca, cepmgBusca, sizeof(busca) - 1);
    paraMinusculo(busca);
    removerAcentos(busca);

    int encontrada = 0;

    for (int i = 0; i < totalLojas; i++)
    {
        char cepmgTemp[100];
        strncpy(cepmgTemp, lojas[i].cepmg, sizeof(cepmgTemp) - 1);
        paraMinusculo(cepmgTemp);
        removerAcentos(cepmgTemp);

        if (strstr(cepmgTemp, busca) != NULL)
        {
            printf("\n=== COLÉGIO ENCONTRADO ===\n");
            printf("CEPMG: %s\n", lojas[i].cepmg);
            printf("Loja mais próxima: %s\n", lojas[i].nome);
            printf("Vendedora: %s\n", lojas[i].vendedora);
            printf("Contato: %s\n", lojas[i].contato);
            printf("Endereço: %s\n", lojas[i].endereco);
            printf("Cidade: %s\n", lojas[i].cidade);
            printf("Estado: %s\n", lojas[i].estado);
            printf("CEP: %s\n\n", lojas[i].cep);

            encontrada = 1;
        }
    }

    if (!encontrada)
    {
        printf("Nenhum colégio encontrado.\n");
    }
}

void buscarPorLocal(const char *localBusca)
{
    char busca[100];

    strncpy(busca, localBusca, sizeof(busca) - 1);
    busca[sizeof(busca) - 1] = '\0';

    paraMinusculo(busca);
    removerAcentos(busca);

    int cont = 0;
    int encontrada = 0;

    for (int i = 0; i < totalLojas; i++)
    {
        char cidadeTemp[100];
        char estadoTemp[20];

        strncpy(cidadeTemp, lojas[i].cidade, sizeof(cidadeTemp) - 1);
        cidadeTemp[sizeof(cidadeTemp) - 1] = '\0';

        strncpy(estadoTemp, lojas[i].estado, sizeof(estadoTemp) - 1);
        estadoTemp[sizeof(estadoTemp) - 1] = '\0';

        // padroniza tudo
        paraMinusculo(cidadeTemp);
        paraMinusculo(estadoTemp);

        removerAcentos(cidadeTemp);
        removerAcentos(estadoTemp);

        // busca parcial
        if (((strcmp(busca, "anapolis") == 0) ? (strcmp(cidadeTemp, busca) == 0)
                                              : (strstr(cidadeTemp, busca) != NULL)) ||
            strstr(estadoTemp, busca) != NULL)
        {
            // imprime cabeçalho só uma vez
            if (!encontrada)
            {
                printf("\n=== LOJAS ENCONTRADAS NA LOCALIZAÇÃO ===\n");
            }

            cont++;

            printf("\nLoja %d\n", cont);
            printf("Nome da loja: %s\n", lojas[i].nome);
            printf("Vendedora: %s\n", lojas[i].vendedora);
            printf("Cidade: %s\n", lojas[i].cidade);
            printf("Estado: %s\n", lojas[i].estado);
            printf("Contato: %s\n", lojas[i].contato);
            printf("Endereço: %s\n", lojas[i].endereco);
            printf("CEPMG: %s\n", lojas[i].cepmg);
            printf("CEP: %s\n", lojas[i].cep);

            printf("----------------------------------------\n");

            encontrada = 1;
        }
    }

    if (encontrada)
    {
        printf("\nTotal de lojas encontradas: %d\n", cont);
    }
    else
    {
        printf("Nenhuma loja encontrada.\n");
    }
}

void listarLojas()
{ // RF04
    for (int i = 0; i < totalLojas; i++)
    {
        printf("\nLoja %d:\n", i + 1);
        printf("Nome: %s\n", lojas[i].nome);
        printf("Vendedora: %s\n", lojas[i].vendedora);
        printf("Cidade: %s\n", lojas[i].cidade);
        printf("Estado: %s\n", lojas[i].estado);
    }
}