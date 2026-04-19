#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lojas.h"

int main() {
    carregarLojas("dados.json");

    int perfil; // RF01
    int opcao;
    char nomedaloja[100]; // RF02

    printf("\n========== LOCALIZADOR COMERCIAL ==========\n");
    printf("Escolha o perfil:\n");
    printf("1 - Atendente\n");
    printf("2 - Administrador\n");
    printf("Opcao: ");
    scanf("%d", &perfil);

    printf("\n============= MENU =============\n"); // obs.: MENU completo
    printf("1 - Buscar pelo nome da loja\n"); // Funcionando
    printf("2 - Buscar pelo Colégio Militar\n"); // ainda nao funcionando corretamente - RF03
    printf("3 - Buscar loja pela cidade ou Estado\n"); // ainda nao funcionando corretamente - RF05
    printf("4 - Listar todas as lojas\n"); // feito - RF04
    // printf para exibir todas as lojas no maps

    if (perfil == 2) {
        printf("5 - Adicionar loja\n");
        printf("6 - Remover loja\n");
    }

    printf("Opcao: ");
    scanf("%d", &opcao); // do menu

    // busca pelo nome da loja --> criar func e mover para funcoes.c depois
    // funcionando
    if (opcao == 1) {
        printf("Digite o nome da loja:\n");

        scanf(" ");
        fgets(nomedaloja, sizeof(nomedaloja), stdin);
        nomedaloja[strcspn(nomedaloja, "\n")] = '\0';

        char busca[100];
        strcpy(busca, nomedaloja);
        paraMinusculo(busca);

        int encontrada = 0;

        for (int i = 0; i < totalLojas; i++) {
            char nomeTemp[100];
            strcpy(nomeTemp, lojas[i].nome);
            paraMinusculo(nomeTemp);

            if (strstr(nomeTemp, busca) != NULL) {
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

        if (!encontrada) {
            printf("Nenhuma loja encontrada.\n");
        }
    }

    // Listagem de lojas --> criar func e mover para funcoes.c depois
    // funcionando -  RF04
    else if (opcao == 2) {
        for (int i = 0; i < totalLojas; i++) {
            printf("\nLoja %d:\n", i + 1);
            printf("Nome: %s\n", lojas[i].nome);
            printf("Vendedora: %s\n", lojas[i].vendedora);
        }
    }

    // perfil admin
    else if (perfil == 2 && opcao == 3) {
        // adicionarLoja(); // funcao ainda nao criada - RF06
    }

    else if (perfil == 2 && opcao == 4) {
        // removerLoja(); // funcao ainda nao criada - RF06
    }

    else {
        printf("Opcao invalida.\n");
    }

    salvarLojas("dados.json"); // salvar alteracoes realizadas no struct para o .json

    return 0;
}