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
            "            background: #f5f5f5;\n"
            "        }\n"

            "        #topo {\n"
            "            background: #7f1d1d;\n"
            "            padding: 12px;\n"
            "            display: flex;\n"
            "            gap: 10px;\n"
            "            align-items: center;\n"
            "            flex-wrap: wrap;\n"
            "        }\n"

            "        #caixaPesquisa {\n"
            "            position: relative;\n"
            "            display: flex;\n"
            "            align-items: center;\n"
            "        }\n"

            "#pesquisa {\n"
            " padding: 10px 10px 10px 40px;\n"
            " width: 320px;\n"
            " border-radius: 8px;\n"
            " border: none;\n"
            " font-size: 14px;\n"
            " background-color: white;\n"
            "background: #fff url(\"https://cdn-icons-png.flaticon.com/512/622/622669.png\") no-repeat 12px center;\n"
            "background-size: 18px;\n"
            " background-repeat: no-repeat;\n"
            "  background-position: 12px center;\n"
            "}\n"

            "        .tipoBusca {\n"
            "            padding: 10px 16px;\n"
            "            border: none;\n"
            "            border-radius: 8px;\n"
            "            cursor: pointer;\n"
            "            background: #991b1b;\n"
            "            color: white;\n"
            "            font-weight: bold;\n"
            "        }\n"

            "        .tipoBusca.ativo {\n"
            "            background: #dc2626;\n"
            "        }\n"

            "        button {\n"
            "            padding: 10px 16px;\n"
            "            border: none;\n"
            "            border-radius: 8px;\n"
            "            cursor: pointer;\n"
            "            background: #991b1b;\n"
            "            color: white;\n"
            "            font-weight: bold;\n"
            "            transition: 0.2s;\n"
            "        }\n"

            "        button:hover {\n"
            "            background: #b91c1c;\n"
            "        }\n"

            "        #map {\n"
            "            width: 100%%;\n"
            "            height: calc(100vh - 70px);\n"
            "        }\n"

            "        #painel {\n"
            "            position: absolute;\n"
            "            top: 90px;\n"
            "            right: 15px;\n"
            "            z-index: 999;\n"
            "            background: white;\n"
            "            padding: 15px;\n"
            "            border-radius: 12px;\n"
            "            width: 280px;\n"
            "            max-height: 350px;\n"
            "            overflow-y: auto;\n"
            "            box-shadow: 0 4px 15px rgba(0,0,0,0.2);\n"
            "        }\n"

            "        #painel h3 {\n"
            "            margin-top: 0;\n"
            "            color: #7f1d1d;\n"
            "        }\n"

            "        #painel ul {\n"
            "            padding-left: 18px;\n"
            "        }\n"

            "        #resultadoBusca {\n"
            "            position: absolute;\n"
            "            top: 130px;\n"
            "            left: 15px;\n"
            "            z-index: 999;\n"
            "            background: white;\n"
            "            padding: 15px;\n"
            "            border-radius: 12px;\n"
            "            width: 300px;\n"
            "            max-height: 350px;\n"
            "            overflow-y: auto;\n"
            "            box-shadow: 0 4px 15px rgba(0,0,0,0.2);\n"
            "            display: none;\n"
            "        }\n"

            "        #resultadoBusca h3 {\n"
            "            margin-top: 0;\n"
            "            color: #7f1d1d;\n"
            "        }\n"

            "        #resultadoBusca ul {\n"
            "            padding-left: 18px;\n"
            "        }\n"

            "        #fecharPainel {\n"
            "            float: right;\n"
            "            background: #dc2626;\n"
            "            border: none;\n"
            "            color: white;\n"
            "            border-radius: 5px;\n"
            "            cursor: pointer;\n"
            "            padding: 4px 8px;\n"
            "        }\n"

            "    </style>\n"

            "</head>\n"

            "<body>\n"

            "<div id='topo'>\n"

            "    <div id='caixaPesquisa'>\n"
            "        <input type='text' id='pesquisa' placeholder='Pesquisar loja, cidade ou CEPMG...'>\n"
            "    </div>\n"

            "    <button class='tipoBusca ativo' onclick=\"alterarTipo('nome', this)\">Nome</button>\n"
            "    <button class='tipoBusca' onclick=\"alterarTipo('cidade', this)\">Cidade</button>\n"
            "    <button class='tipoBusca' onclick=\"alterarTipo('cepmg', this)\">CEPMG</button>\n"
            "    <button onclick='location.reload()'>Atualizar mapa</button>\n"

            "</div>\n"

            "<div id='map'></div>\n"

            "<div id='painel' style='display:none;'>\n"
            "    <h3>Lojas sem coordenadas</h3>\n"
            "    <ul id='listaSemCoordenadas'></ul>\n"
            "</div>\n"

            "<div id='resultadoBusca'>\n"
            "    <button id='fecharPainel' onclick='fecharResultados()'>X</button>\n"
            "    <h3>Resultados encontrados</h3>\n"
            "    <ul id='listaResultados'></ul>\n"
            "</div>\n"

            "<script>\n"

            "var tipoBuscaAtual = 'nome';\n"

            "function alterarTipo(tipo, botao) {\n"

            "    tipoBuscaAtual = tipo;\n"

            "    let botoes = document.getElementsByClassName('tipoBusca');\n"

            "    for (let i = 0; i < botoes.length; i++) {\n"
            "        botoes[i].classList.remove('ativo');\n"
            "    }\n"

            "    botao.classList.add('ativo');\n"

            "}\n"

            "var map = L.map('map').setView([-16.5, -49.3], 7);\n"

            "L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\n"
            "    attribution: '&copy; OpenStreetMap'\n"
            "}).addTo(map);\n"

            "var marcadorAzul = new L.Icon({\n"
            "    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-blue.png',\n"
            "    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',\n"
            "    iconSize: [25, 41],\n"
            "    iconAnchor: [12, 41],\n"
            "    popupAnchor: [1, -34],\n"
            "    shadowSize: [41, 41]\n"
            "});\n"

            "var marcadorVermelho = new L.Icon({\n"
            "    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-red.png',\n"
            "    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',\n"
            "    iconSize: [25, 41],\n"
            "    iconAnchor: [12, 41],\n"
            "    popupAnchor: [1, -34],\n"
            "    shadowSize: [41, 41]\n"
            "});\n"

            "var marcadores = [];\n"

            "var lojasSemCoordenadas = [];\n"

    );

    for (int i = 0; i < totalLojas; i++)
    {
        if (
            lojas[i].lat == 0 ||
            lojas[i].lng == 0 ||
            lojas[i].lat == -9999 ||
            lojas[i].lng == -9999)
        {
            fprintf(html,
                    "lojasSemCoordenadas.push('%s');\n",
                    lojas[i].nome);

            continue;
        }

        fprintf(html,

                "var marker%d = L.marker([%lf, %lf], {icon: marcadorAzul}).addTo(map)\n"

                ".bindPopup(`"

                "<div style='min-width:280px;'>"

                "<h3 style='margin:0;color:#7f1d1d;'>%s</h3>"

                "<hr>"

                "<b>Vendedora:</b> %s<br>"

                "<b>Cidade:</b> %s - %s<br>"

                "<b>Contato:</b> %s<br>"

                "<b>CEPMG:</b> %s<br>"

                "<b>Endereço:</b><br>%s"

                "</div>"

                "`);\n"

                "marcadores.push({\n"

                "    nome: '%s',\n"

                "    cidade: '%s',\n"

                "    estado: '%s',\n"

                "    cepmg: '%s',\n"

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
                lojas[i].endereco,

                lojas[i].nome,
                lojas[i].cidade,
                lojas[i].estado,
                lojas[i].cepmg,
                i);
    }

    fprintf(html,

            "if (lojasSemCoordenadas.length > 0) {"

            "document.getElementById('painel').style.display = 'block';"

            "let lista ="
            " document.getElementById('listaSemCoordenadas');"

            "for (let i = 0; i < lojasSemCoordenadas.length; i++) {"

            "let item ="
            "  document.createElement('li');"

            "item.innerText ="
            " lojasSemCoordenadas[i];"

            "lista.appendChild(item);"
            "}"
            "}"

            "function fecharResultados() {\n"

            "    document.getElementById('resultadoBusca').style.display = 'none';\n"

            "}\n"
            "function normalizarTexto(texto) {\n"
            "    return texto\n"
            "        .toLowerCase()\n"
            "        .normalize('NFD')\n"
            "        .replace(/[\\u0300-\\u036f]/g, '');\n"
            "}\n"

            "  function buscarLoja() {\n"

            " var texto = normalizarTexto(\n"
            "  document.getElementById('pesquisa').value\n"
            "    );\n"

            " if (texto.trim() === '') {"

            "  for (let i = 0; i < marcadores.length; i++) {"
            "   marcadores[i].marker.setIcon(marcadorAzul);"
            " }"

            " document.getElementById('resultadoBusca').style.display = 'none';"

            "  return;"
            " }"

            "    let encontrados = 0;\n"

            "    let grupo = [];\n"

            "    let listaResultados = document.getElementById('listaResultados');\n"

            "    listaResultados.innerHTML = '';\n"

            "    for (let i = 0; i < marcadores.length; i++) {\n"

            "        marcadores[i].marker.setIcon(marcadorAzul);\n"

            "        let campo = '';\n"

            "        if (tipoBuscaAtual == 'nome') {\n"
            "            campo = normalizarTexto(marcadores[i].nome);\n"
            "        }\n"

            "        if (tipoBuscaAtual == 'cidade') {\n"
            "            campo = normalizarTexto(marcadores[i].cidade);\n"
            "        }\n"

            "        if (tipoBuscaAtual == 'cepmg') {\n"
            "            campo = normalizarTexto(marcadores[i].cepmg);\n"
            "        }\n"

            "        if (campo.includes(texto)) {\n"

            "            marcadores[i].marker.setIcon(marcadorVermelho);\n"

            "            grupo.push(marcadores[i].marker);\n"

            "            encontrados++;\n"

            "            let item = document.createElement('button');\n"

            "            item.innerText = marcadores[i].nome;\n"

            "            item.style.width = '100%%';\n"
            "            item.style.marginBottom = '8px';\n"
            "            item.style.textAlign = 'left';\n"
            "            item.style.padding = '10px';\n"
            "            item.style.borderRadius = '8px';\n"
            "            item.style.border = 'none';\n"
            "            item.style.background = '#dc2626';\n"
            "            item.style.color = 'white';\n"
            "            item.style.fontWeight = 'bold';\n"
            "            item.style.cursor = 'pointer';\n"
            "            item.style.transition = '0.2s';\n"

            "            item.onmouseover = function() {\n"
            "                this.style.background = '#991b1b';\n"
            "            };\n"

            "            item.onmouseout = function() {\n"
            "                this.style.background = '#dc2626';\n"
            "            };\n"
            "            item.onclick = function() {\n"

            "                map.setView(\n"
            "                    marcadores[i].marker.getLatLng(),\n"
            "                    15\n"
            "                );\n"

            "                marcadores[i].marker.openPopup();\n"

            "            };\n"

            "            listaResultados.appendChild(item);\n"

            "        }\n"

            "    }\n"

            "    if (grupo.length > 0) {\n"

            "        var bounds = L.featureGroup(grupo).getBounds();\n"

            "        map.fitBounds(bounds, {padding:[50,50]});\n"

            "        document.getElementById('resultadoBusca').style.display = 'block';\n"

            "    }\n"

            "    if (encontrados == 0) {\n"

            "        alert('Nenhuma loja encontrada!');\n"

            "        document.getElementById('resultadoBusca').style.display = 'none';\n"

            "    }\n"

            "}\n"
            "document.getElementById('pesquisa').addEventListener('input', function() {\n"
            "    buscarLoja();\n"
            "});\n"
            "document.getElementById('pesquisa').addEventListener('keypress', function(e) {\n"

            "    if (e.key === 'Enter') {\n"

            "        buscarLoja();\n"

            "    }\n"

            "});\n"

            "</script>\n"

            "</body>\n"

            "</html>\n"

    );

    fclose(html);

}