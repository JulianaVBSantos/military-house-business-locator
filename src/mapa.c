#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lojas.h"

void gerarMapa()
{
    FILE *html = fopen("mapa.html", "w");

    if (html == NULL)
    {
        printf("Erro ao criar mapa.html\n");
        return;
    }

    fprintf(html,
            "<!DOCTYPE html>\n"
            "<html lang='pt-BR'>\n"
            "<head>\n"
            "    <meta charset='UTF-8'>\n"
            "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
            "    <title>Mapa das Lojas</title>\n"

            "    <link rel='stylesheet' href='https://unpkg.com/leaflet/dist/leaflet.css'/>\n"
            "    <script src='https://unpkg.com/leaflet/dist/leaflet.js'></script>\n"

            "    <style>\n"

            "        body {\n"
            "            margin: 0;\n"
            "            padding: 0;\n"
            "            font-family: Arial, sans-serif;\n"
            "        }\n"

            "        #topo {\n"
            "            background: #1f2937;\n"
            "            padding: 10px;\n"
            "            display: flex;\n"
            "            gap: 10px;\n"
            "            align-items: center;\n"
            "        }\n"

            "        #pesquisa {\n"
            "            padding: 8px;\n"
            "            width: 250px;\n"
            "            border-radius: 6px;\n"
            "            border: none;\n"
            "        }\n"

            "        button {\n"
            "            padding: 8px 14px;\n"
            "            border: none;\n"
            "            border-radius: 6px;\n"
            "            cursor: pointer;\n"
            "            background: #2563eb;\n"
            "            color: white;\n"
            "            font-weight: bold;\n"
            "        }\n"

            "        button:hover {\n"
            "            background: #1d4ed8;\n"
            "        }\n"

            "        #map {\n"
            "            width: 100%%;\n"
            "            height: calc(100vh - 60px);\n"
            "        }\n"

            "    </style>\n"

            "</head>\n"

            "<body>\n"

            "<div id='topo'>\n"

            "    <input type='text' id='pesquisa' placeholder='Pesquisar loja...'>\n"

            "    <button onclick='buscarLoja()'>Pesquisar</button>\n"

            "    <button onclick='location.reload()'>Atualizar mapa</button>\n"

            "</div>\n"

            "<div id='map'></div>\n"

            "<script>\n"

            "var map = L.map('map').setView([-16.5, -49.3], 7);\n"

            "L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\n"
            "    attribution: '&copy; OpenStreetMap'\n"
            "}).addTo(map);\n"

            "var marcadores = [];\n"

    );

    for (int i = 0; i < totalLojas; i++)
    {
        if (
            lojas[i].lat == 0 ||
            lojas[i].lng == 0 ||
            lojas[i].lat == -9999 ||
            lojas[i].lng == -9999)
        {
            continue;
        }

        fprintf(html,

                "var marker%d = L.marker([%lf, %lf]).addTo(map)\n"
                ".bindPopup(`"
                "<b>%s</b><br>"
                "Vendedora: %s<br>"
                "Cidade: %s - %s<br>"
                "Contato: %s<br>"
                "CEPMG: %s"
                "`);\n"

                "marcadores.push({\n"
                "    nome: '%s',\n"
                "    marker: marker%d\n"
                "});\n",

                i,
                lojas[i].lat,
                lojas[i].lng,
                lojas[i].nome,
                lojas[i].vendedora,
                lojas[i].cidade,
                lojas[i].estado,
                lojas[i].contato,
                lojas[i].cepmg,
                lojas[i].nome,
                i);
    }

fprintf(html,

    "function buscarLoja() {\n"

    "    var texto = document.getElementById('pesquisa').value.toLowerCase();\n"

    "    for (let i = 0; i < marcadores.length; i++) {\n"

    "        if (marcadores[i].nome.toLowerCase().includes(texto)) {\n"

    "            map.setView(marcadores[i].marker.getLatLng(), 15);\n"

    "            marcadores[i].marker.openPopup();\n"

    "            return;\n"

    "        }\n"

    "    }\n"

    "    alert('Loja não encontrada!');\n"

    "}\n"

    "</script>\n"
    "</body>\n"
    "</html>"
);;

    fclose(html);

    printf("Mapa gerado com sucesso!\n");

    system("start mapa.html");
}