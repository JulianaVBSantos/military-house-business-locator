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
    printf("1 - Buscar pelo nome da loja\n"); // Funcionando - RF02
    printf("2 - Buscar pelo Colégio Militar\n"); // Funcionando - RF03
    printf("3 - Buscar loja pela cidade ou Estado\n"); // Ainda não implementado - RF05
    printf("4 - Listar todas as lojas\n"); // feito - RF04

    if (perfil == 2) {
        printf("5 - Adicionar loja\n");
        printf("6 - Remover loja\n");
    }

    printf("Opcao: ");
    scanf("%d", &opcao); // do menu

    if (opcao == 1) { // RF02
        printf("Digite o nome da loja:\n");

        scanf(" ");
        fgets(nomedaloja, sizeof(nomedaloja), stdin);
        nomedaloja[strcspn(nomedaloja, "\n")] = '\0';

        buscarPorNome(nomedaloja);
    }

    // busca pelo colégio militar - RF03
    else if (opcao == 2) {
        char cepmg[100];

        printf("Digite o nome do Colégio Militar:\n");

        scanf(" ");
        fgets(cepmg, sizeof(cepmg), stdin);
        cepmg[strcspn(cepmg, "\n")] = '\0';

        buscarPorCEPMG(cepmg);
    }

    // busca por cidade ou estado - RF05
    // ainda não funcionando
    else if (opcao == 3) {
        char local[100];

        printf("Digite a cidade ou estado:\n");

        scanf(" ");
        fgets(local, sizeof(local), stdin);
        local[strcspn(local, "\n")] = '\0';

        buscarPorLocal(local);
    }

    else if (opcao == 4) { // RF04
        listarLojas();
    }

    // perfil admin
    else if (perfil == 2 && opcao == 5) {
        // adicionarLoja(); // funcao ainda nao criada - RF06
    }

    else if (perfil == 2 && opcao == 6) {
        // removerLoja(); // funcao ainda nao criada - RF06
    }

    else {
        printf("Opcao invalida.\n");
    }

    salvarLojas("dados.json"); // salvar alteracoes realizadas no struct para o .json

    return 0;
}