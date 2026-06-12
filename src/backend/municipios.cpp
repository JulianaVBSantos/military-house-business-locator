#include <cmath>
#include <fstream>
#include <vector>
#include <QString>
#include <QCoreApplication>
#include <QDebug>

#include "cJSON.h"
#include "lojas.h"

static std::vector<Municipio> municipios;

static double haversine(
    double lat1,
    double lon1,
    double lat2,
    double lon2)
{
    const double R = 6371.0;

    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    double a =
        sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1 * M_PI / 180.0) *
            cos(lat2 * M_PI / 180.0) *
            sin(dLon / 2) *
            sin(dLon / 2);

    double c =
        2 * atan2(
                sqrt(a),
                sqrt(1 - a));

    return R * c;
}

static QString normalizar(QString texto)
{
    texto = texto.toLower().trimmed();

    texto.replace("á", "a");
    texto.replace("à", "a");
    texto.replace("â", "a");
    texto.replace("ã", "a");

    texto.replace("é", "e");
    texto.replace("ê", "e");

    texto.replace("í", "i");

    texto.replace("ó", "o");
    texto.replace("ô", "o");
    texto.replace("õ", "o");

    texto.replace("ú", "u");

    texto.replace("ç", "c");

    return texto;
}

void carregarMunicipios()
{
    municipios.clear();

    QString caminho =
        QCoreApplication::applicationDirPath() +
        "/data/municipios_go_regiao.json";

    std::ifstream arq(
        caminho.toStdString());

    if (!arq.is_open())
    {
        qDebug() << "ERRO AO ABRIR:" << caminho;
        return;
    }

    std::string json(
        (std::istreambuf_iterator<char>(arq)),
        std::istreambuf_iterator<char>());

    cJSON *root =
        cJSON_Parse(json.c_str());

    if (!root)
    {
        qDebug() << "Erro ao interpretar JSON";
        return;
    }

    int total =
        cJSON_GetArraySize(root);

    for (int i = 0; i < total; i++)
    {
        cJSON *item =
            cJSON_GetArrayItem(root, i);

        Municipio m = {};

        strcpy(
            m.nome,
            cJSON_GetObjectItem(item, "nome")->valuestring);

        strcpy(
            m.uf,
            cJSON_GetObjectItem(item, "uf")->valuestring);

        m.latitude =
            cJSON_GetObjectItem(item, "latitude")->valuedouble;

        m.longitude =
            cJSON_GetObjectItem(item, "longitude")->valuedouble;

        municipios.push_back(m);
    }

    qDebug() << "Municipios carregados:"
             << municipios.size();

    cJSON_Delete(root);
}

int buscarLojaMaisProximaPorMunicipio(
    const char *cidade,
    Loja resultado[])
{
    QString busca =
        normalizar(
            QString::fromUtf8(cidade));

    qDebug() << "Cidade recebida:" << busca;

    Municipio *municipio = nullptr;

    // 1º tenta correspondência exata
    for (auto &m : municipios)
    {
        QString nome =
            normalizar(
                QString::fromUtf8(m.nome));

        if (nome.startsWith(busca))
        {
            municipio = &m;

            qDebug() << "Municipio encontrado (parcial):"
                     << m.nome;
            qDebug() << "Municipio:"
                     << municipio->nome
                     << municipio->latitude
                     << municipio->longitude;
            break;
        }
    }

    // 2º tenta correspondência parcial
    if (!municipio)
    {
        for (auto &m : municipios)
        {
            QString nome =
                normalizar(
                    QString::fromUtf8(m.nome));

            if (nome.startsWith(busca))
            {
                municipio = &m;

                qDebug() << "Municipio encontrado (parcial):"
                         << m.nome;

                break;
            }
        }
    }

    if (!municipio)
    {
        qDebug() << "Municipio nao encontrado:"
                 << busca;

        return 0;
    }

    qDebug() << "Total lojas:" << totalLojas;

    double menor = 999999999.0;
    int indice = -1;

    for (int i = 0; i < totalLojas; i++)
    {
        double dist =
            haversine(
                municipio->latitude,
                municipio->longitude,
                lojas[i].lat,
                lojas[i].lng);

        if (dist < menor)
        {
            menor = dist;
            indice = i;
        }
    }

    qDebug() << "Indice encontrado:" << indice;
    qDebug() << "Menor distancia:" << menor;

    if (indice < 0)
        return 0;

    qDebug() << "Loja selecionada:"
             << lojas[indice].nome
             << lojas[indice].cidade;

    resultado[0] =
        lojas[indice];

    return 1;
}

int getTotalMunicipios()
{
    return (int)municipios.size();
}

const Municipio* getMunicipio(
    int indice)
{
    if (
        indice < 0 ||
        indice >= (int)municipios.size())
    {
        return nullptr;
    }

    return &municipios[indice];
}