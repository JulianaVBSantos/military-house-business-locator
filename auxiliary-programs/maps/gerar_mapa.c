// Integração com mapa interativo utilizando
// Leaflet + OpenStreetMap

#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE *html = fopen("mapa.html", "w");

    if (!html) {
        printf("Erro ao criar arquivo HTML.\n");
        return 1;
    }

    fprintf(html,

"<!DOCTYPE html>\n"
"<html lang='pt-BR'>\n"

"<head>\n"
"    <meta charset='utf-8'>\n"
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
"            color: white;\n"
"            padding: 12px;\n"
"            display: flex;\n"
"            align-items: center;\n"
"            gap: 10px;\n"
"        }\n"

"        #map {\n"
"            height: calc(100vh - 60px);\n"
"            width: 100%%;\n"
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

"        .popup-title {\n"
"            font-size: 16px;\n"
"            font-weight: bold;\n"
"            margin-bottom: 8px;\n"
"        }\n"

"        .maps-link {\n"
"            display: inline-block;\n"
"            margin-top: 10px;\n"
"            text-decoration: none;\n"
"            color: #2563eb;\n"
"            font-weight: bold;\n"
"        }\n"

"    </style>\n"

"</head>\n"

"<body>\n"

"    <div id='topo'>\n"

"        <button onclick='carregarMapa()'>Atualizar mapa</button>\n"

"        <span id='status'>Carregando lojas...</span>\n"

"    </div>\n"

"    <div id='map'></div>\n"

"    <script>\n"

"        // Inicialização do mapa\n"
"        var map = L.map('map').setView([-16.5, -49.3], 7);\n"

"        // Camada OpenStreetMap\n"
"        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\n"
"            attribution: '&copy; OpenStreetMap'\n"
"        }).addTo(map);\n"

"        // Vetor de marcadores\n"
"        var markers = [];\n"

"        // Remove todos os marcadores\n"
"        function limparMapa() {\n"

"            markers.forEach(marker => {\n"
"                map.removeLayer(marker);\n"
"            });\n"

"            markers = [];\n"

"        }\n"

"        // Carrega lojas do JSON\n"
"        function carregarMapa() {\n"

"            limparMapa();\n"

"            fetch('lojas_com_coord.json')\n"

"            .then(response => response.json())\n"

"            .then(lojas => {\n"

"                let total = 0;\n"

"                lojas.forEach(loja => {\n"

"                    // Ignora lojas sem coordenadas\n"
"                    if (loja.lat == null || loja.lng == null)\n"
"                        return;\n"

"                    let popup = `\n"

"                        <div class='popup-title'>${loja.nome}</div>\n"

"                        <b>Endereço:</b><br>\n"
"                        ${loja.endereco || '-'}<br><br>\n"

"                        <b>Cidade:</b><br>\n"
"                        ${loja.cidade || '-'} - ${loja.estado || '-'}<br><br>\n"

"                        <b>Contato:</b><br>\n"
"                        ${loja.contato || '-'}<br><br>\n"

"                        <b>Vendedora:</b><br>\n"
"                        ${loja.vendedora || '-'}<br><br>\n"

"                        <b>CEPMG:</b><br>\n"
"                        ${loja.cepmg || '-'}<br><br>\n"

"                        <a\n"
"                            class='maps-link'\n"
"                            target='_blank'\n"
"                            href='https://www.google.com/maps?q=${loja.lat},${loja.lng}'>\n"
"                            Abrir no Google Maps\n"
"                        </a>\n"

"                    `;\n"

"                    let marker = L.marker([loja.lat, loja.lng])\n"
"                        .addTo(map)\n"
"                        .bindPopup(popup);\n"

"                    markers.push(marker);\n"

"                    total++;\n"

"                });\n"

"                // Ajusta zoom automaticamente\n"
"                if (markers.length > 0) {\n"

"                    let grupo = L.featureGroup(markers);\n"

"                    map.fitBounds(grupo.getBounds(), {\n"
"                        padding: [30, 30]\n"
"                    });\n"

"                }\n"

"                document.getElementById('status').innerText =\n"
"                    total + ' loja(s) carregada(s)';\n"

"            })\n"

"            .catch(error => {\n"

"                console.error(error);\n"

"                document.getElementById('status').innerText =\n"
"                    'Erro ao carregar JSON';\n"

"                alert('Erro ao carregar lojas_com_coord.json');\n"

"            });\n"

"        }\n"

"        // Carrega automaticamente ao abrir\n"
"        carregarMapa();\n"

"    </script>\n"

"</body>\n"
"</html>\n"

    );

    fclose(html);

    printf("Mapa gerado com sucesso!\n");

    system("start mapa.html");

    return 0;
}