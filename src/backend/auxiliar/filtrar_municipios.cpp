#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "cJSON.h"

int main()
{
    std::ifstream arqEntrada("municipios_com_estado.json");

    if (!arqEntrada.is_open())
    {
        std::cout << "Erro ao abrir arquivo.\n";
        return 1;
    }

    std::string json(
        (std::istreambuf_iterator<char>(arqEntrada)),
        std::istreambuf_iterator<char>());

    arqEntrada.close();

    cJSON* root = cJSON_Parse(json.c_str());

    if (!root)
    {
        std::cout << "Erro ao ler JSON.\n";
        return 1;
    }

    cJSON* novoArray = cJSON_CreateArray();

    std::set<std::string> estadosPermitidos =
    {
        "GO",
        "DF",
        "MG",
        "MT",
        "MS",
        "TO"
    };

    int total = cJSON_GetArraySize(root);

    for (int i = 0; i < total; i++)
    {
        cJSON* item =
            cJSON_GetArrayItem(root, i);

        cJSON* uf =
            cJSON_GetObjectItem(item, "uf");

        if (!uf || !cJSON_IsString(uf))
            continue;

        std::string sigla = uf->valuestring;

        if (estadosPermitidos.count(sigla))
        {
            cJSON_AddItemToArray(
                novoArray,
                cJSON_Duplicate(item, 1));
        }
    }

    char* jsonFiltrado =
        cJSON_Print(novoArray);

    std::ofstream arqSaida(
        "municipios_go_regiao.json");

    arqSaida << jsonFiltrado;

    arqSaida.close();

    std::cout
        << "Arquivo filtrado criado com sucesso.\n";

    free(jsonFiltrado);

    cJSON_Delete(novoArray);
    cJSON_Delete(root);

    return 0;
}