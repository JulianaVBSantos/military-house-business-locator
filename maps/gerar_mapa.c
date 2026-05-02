// Integração com mapa interativo (Leaflet + OpenStreetMap) foi iniciada,
// porém temporariamente descontinuada nesta etapa do projeto.
// A abordagem baseada em geocodificação apresentou baixa precisão devido
// à falta de padronização dos endereços e dependência de serviços externos.
// Os arquivos foram mantidos na pasta "maps" para possível retomada futura.

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *html = fopen("mapa.html", "w");

    if (!html) {
        printf("Erro ao criar HTML\n");
        return 1;
    }

    fprintf(html,
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<meta charset='utf-8'>\n"
    "<link rel='stylesheet' href='https://unpkg.com/leaflet/dist/leaflet.css'/>\n"
    "<script src='https://unpkg.com/leaflet/dist/leaflet.js'></script>\n"
    "<style>#map{height:100vh;}</style>\n"
    "</head>\n"
    "<body>\n"

    "<button onclick='carregar()'>Atualizar mapa</button>\n"
    "<div id='map'></div>\n"

    "<script>\n"

    "var map = L.map('map').setView([-16.5, -49.3], 7);\n"

    "L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\n"
    " attribution: '© OpenStreetMap'\n"
    "}).addTo(map);\n"

    "var markers = [];\n"

    "function limparMapa() {\n"
    "  markers.forEach(m => map.removeLayer(m));\n"
    "  markers = [];\n"
    "}\n"

    "function carregar() {\n"
    " limparMapa();\n"

    " fetch('lojas_com_coord.json')\n"
    " .then(res => res.json())\n"
    " .then(lojas => {\n"

    "   lojas.forEach(loja => {\n"

    "     if (!loja.lat || !loja.lng) return;\n"

    "     let marker = L.marker([loja.lat, loja.lng])\n"
    "       .addTo(map)\n"
    "       .bindPopup(`\n"
    "         <b>${loja.nome}</b><br>\n"
    "         ${loja.endereco || ''}<br>\n"
    "         ${loja.cidade || ''} - ${loja.estado || ''}<br>\n"
    "         ${loja.contato || ''}<br>\n"
    "         ${loja.vendedora || ''}<br>\n"
    "         ${loja.cepmg || ''}\n"
    "       `);\n"

    "     markers.push(marker);\n"

    "   });\n"

    " });\n"
    "}\n"

    "carregar();\n"

    "</script>\n"
    "</body>\n"
    "</html>\n"
    );

    fclose(html);

    printf("Mapa gerado com sucesso!\n");

    system("start mapa.html");

    return 0;
}