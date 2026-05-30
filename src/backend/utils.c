#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lojas.h"
#include "cJSON.h"

Loja lojas[MAX_LOJAS];
int totalLojas = 0;

void paraMinusculo(char *str)
{ // case-insensitive (maiúscula/minúscula)
    for (int i = 0; str[i]; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void lerCampoObrigatorio(char *campo, int tamanho, const char *mensagem)
{
    do
    {
        printf("%s", mensagem);

        fgets(campo, tamanho, stdin);

        if (strchr(campo, '\n') == NULL) // se nao encontrou '\n', limpa o restante
        {
            limparBuffer();
        }

        campo[strcspn(campo, "\n")] = '\0';

        if (strlen(campo) == 0)
        {
            printf("Campo obrigatorio. Tente novamente.\n");
        }

    } while (strlen(campo) == 0);
}

void removerAcentos(char *str)
{
    unsigned char *p = (unsigned char *)str;
    char resultado[1000];
    int j = 0;

    while (*p)
    {
        // letras acentuadas UTF-8 começam com 0xC3
        if (*p == 0xC3)
        {
            p++;

            switch (*p)
            {
            // a
            case 0xA1: // á
            case 0xA0: // à
            case 0xA2: // â
            case 0xA3: // ã
            case 0x81: // Á
            case 0x80: // À
            case 0x82: // Â
            case 0x83: // Ã
                resultado[j++] = 'a';
                break;

            // e
            case 0xA9: // é
            case 0xAA: // ê
            case 0x89: // É
            case 0x8A: // Ê
                resultado[j++] = 'e';
                break;

            // i
            case 0xAD: // í
            case 0x8D: // Í
                resultado[j++] = 'i';
                break;

            // o
            case 0xB3: // ó
            case 0xB4: // ô
            case 0xB5: // õ
            case 0x93: // Ó
            case 0x94: // Ô
            case 0x95: // Õ
                resultado[j++] = 'o';
                break;

            // u
            case 0xBA: // ú
            case 0x9A: // Ú
                resultado[j++] = 'u';
                break;

            // c
            case 0xA7: // ç
            case 0x87: // Ç
                resultado[j++] = 'c';
                break;

            default:
                break;
            }

            p++;
        }
        else
        {
            resultado[j++] = *p;
            p++;
        }
    }

    resultado[j] = '\0';

    strcpy(str, resultado);
}

