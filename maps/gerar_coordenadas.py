# Integração com mapa interativo (Leaflet + OpenStreetMap) foi iniciada,
# porém temporariamente descontinuada nesta etapa do projeto.
# A abordagem baseada em geocodificação apresentou baixa precisão devido
# à falta de padronização dos endereços e dependência de serviços externos.
# Os arquivos foram mantidos na pasta "maps" para possível retomada futura.

import json
import time
import requests

def geocode(endereco):
    url = "https://nominatim.openstreetmap.org/search"
    params = {
        "q": endereco,
        "format": "json",
        "limit": 1
    }

    headers = {
        "User-Agent": "mapa-lojas-faculdade"
    }

    try:
        response = requests.get(url, params=params, headers=headers)
        data = response.json()

        if data:
            return float(data[0]["lat"]), float(data[0]["lon"])
    except:
        pass

    return None, None


with open("lojas.json", "r", encoding="utf-8") as f:
    lojas = json.load(f)

for loja in lojas:

    print("Buscando:", loja["nome"])

    lat, lng = None, None

    # Tentativa 1: CEPMG + cidade
    if loja.get("cepmg") and loja["cepmg"] != "—":
        endereco = f"{loja['cepmg']}, {loja['cidade']}, {loja['estado']}, Brasil"
        lat, lng = geocode(endereco)

    # Tentativa 2: só CEPMG
    if not lat and loja.get("cepmg") and loja["cepmg"] != "—":
        endereco = f"{loja['cepmg']}, Brasil"
        lat, lng = geocode(endereco)

    # Tentativa 3: cidade + estado
    if not lat and loja.get("cidade"):
        endereco = f"{loja['cidade']}, {loja['estado']}, Brasil"
        lat, lng = geocode(endereco)

    # Tentativa 4: CEP
    if not lat and loja.get("cep") and loja["cep"] != "—":
        endereco = f"{loja['cep']}, Brasil"
        lat, lng = geocode(endereco)

    loja["lat"] = lat
    loja["lng"] = lng

    print(" →", lat, lng)

    time.sleep(1)  # evita bloqueio da API

with open("lojas_com_coord.json", "w", encoding="utf-8") as f:
    json.dump(lojas, f, ensure_ascii=False, indent=2)

print("Finalizado com busca por CEPMG!")