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
void paraMinusculo(char *str);

#endif