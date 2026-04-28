#ifndef LOJAS_H
#define LOJAS_H

#define MAX_LOJAS 100

typedef struct {
    char nome[100];
    char contato[30];
    char vendedora[50];
    char endereco[150];
    char cidade[60];
    char estado[3]; // "GO" + '\0'
    char cepmg[100];
    char cep[10]; // "00000-000" + '\0'
} Loja;

extern Loja lojas[MAX_LOJAS];
extern int totalLojas;

void carregarLojas(const char *arquivo);
void salvarLojas(const char *arquivo);
void paraMinusculo(char *str); // case-insensitive (maiúscula/minúscula)
void removerAcentos(char *str); // caso de acentos
void buscarPorNome(const char *nomeBusca); // RF02
void buscarPorCEPMG(const char *cepmgBusca); // RF03
void listarLojas(); // RF04
void buscarPorLocal(const char *localBusca); // RF05

#endif