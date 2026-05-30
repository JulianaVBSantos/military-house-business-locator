import csv
import json

def csv_para_json(arquivo_csv, arquivo_json):
    try:
        with open(arquivo_csv, mode='r', encoding='utf-8') as f:
            leitor = csv.DictReader(f)
            dados = []

            for linha in leitor:
                
                linha_limpa = {
                    chave.strip(): valor.strip() if valor else ""
                    for chave, valor in linha.items()
                }
                dados.append(linha_limpa)

        with open(arquivo_json, mode='w', encoding='utf-8') as f:
            json.dump(dados, f, ensure_ascii=False, indent=2)

        print("Conversão concluída! JSON gerado com sucesso.")

    except Exception as e:
        print(f"Erro: {e}")


if __name__ == "__main__":
    csv_para_json("dados_exemplo.csv", "dados_sample.json")