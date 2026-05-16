#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lojas.h"

#define SENHA_ADMIN "Senha_Fake"

int main()
{
    carregarLojas("dados.json");

    int perfil; // RF01
    int opcao;

    char nomedaloja[100]; // RF02
    char senha[50];

    printf("\n========== LOCALIZADOR COMERCIAL ==========\n");

    do // seleção de perfil - RF01
    {
        printf("Escolha o perfil:\n");
        printf("1 - Perfil Gestor\n");
        printf("2 - Perfil Consulta\n");
        printf("Perfil: ");

        scanf("%d", &perfil);
        getchar();

        if (perfil != 1 && perfil != 2)
        {
            printf("Opção inválida. Insira um perfil existente.\n\n");
        }

    } while (perfil != 1 && perfil != 2);

    if (perfil == 1) // autenticação gestor
    {
        do
        {
            printf("Digite a senha de administrador: ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = '\0'; // remover quebra de linha

            if (strcmp(senha, "0") == 0)
            {
                return 0; // encerra o programa
            }

            if (strcmp(senha, SENHA_ADMIN) != 0) // strcmp -- comparar c/ exatidão
            {
                printf("\nAcesso negado. Senha incorreta.\n");
                printf("\nDigite 0 para cancelar.\n");
            }

        } while (strcmp(senha, SENHA_ADMIN) != 0);

        printf("\nAcesso autorizado.\n");
    }

    do // menu principal
    {
        printf("\n============= MENU =============\n");

        printf("1 - Buscar pelo nome da loja\n");          // RF02 - OK
        printf("2 - Buscar pelo Colégio Militar\n");       // RF03 - OK
        printf("3 - Buscar loja pela cidade ou Estado\n"); // RF05 - OK
        printf("4 - Listar todas as lojas\n");             // RF04 - PARTE 01 - OK
        printf("5 - Visualizar mapa\n");                   // RF04 - PARTE 02

        if (perfil == 1) // perfil gestor - RF06
        {
            printf("6 - Adicionar loja\n");
            printf("7 - Remover loja\n");
            printf("8 - Editar loja\n");
        }

        printf("Opção: ");

        scanf("%d", &opcao); // leitura do menu
        getchar();

        if (opcao < 1 || opcao > 8) // validação das opções do menu
        {
            printf("Opção inválida. Insira uma opção entre 1 e 8.\n");
        }

        if (perfil != 1 && (opcao == 6 || opcao == 7 || opcao == 8)) // impede acesso às funções de gestor
        {
            printf("Acesso negado.\n");
            opcao = -1;
        }

    } while (opcao < 1 || opcao > 8);

    switch (opcao)
    {
    case 1: // RF02
    {
        printf("Digite o nome da loja:\n");
        scanf(" ");
        fgets(nomedaloja, sizeof(nomedaloja), stdin);
        nomedaloja[strcspn(nomedaloja, "\n")] = '\0';
        buscarPorNome(nomedaloja);
        break;
    }

    case 2: // busca pelo colégio militar - RF03
    {
        char cepmg[100];
        printf("Digite o nome do Colégio Militar:\n");
        scanf(" ");
        fgets(cepmg, sizeof(cepmg), stdin);
        cepmg[strcspn(cepmg, "\n")] = '\0';
        buscarPorCEPMG(cepmg);
        break;
    }

    case 3: // busca por cidade ou estado - RF05
    {
        char local[100];
        printf("Digite a cidade ou estado:\n");
        scanf(" ");
        fgets(local, sizeof(local), stdin);
        local[strcspn(local, "\n")] = '\0';
        buscarPorLocal(local);
        break;
    }

    case 4: // RF04
    {
        listarLojas();
        break;
    }

    case 5: // RF04 - PARTE 02
    {
        gerarMapa();
        break;
    }

    // perfil gestor - RF06
    case 6:
    {
        adicionarLoja();
        break;
    }

    case 7:
    {
        removerLoja();
        break;
    }

    case 8:
    {
        editarLoja();
        break;
    }

    default:
    {
        printf("Opção inválida.\n");
    }
    }

    salvarLojas("dados.json"); // salvar alterações realizadas no struct para o .json

    return 0;
}