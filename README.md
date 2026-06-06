# Sistema Localizador Comercial

Sistema desenvolvido como parte de um projeto extensionista com o objetivo de otimizar o atendimento ao cliente por meio da centralização de informações de lojas vinculadas à Casa do Militar.

---

## Funcionalidades

- Busca de lojas por nome  
- Busca por colégio militar  
- Listagem de lojas cadastradas  
- Filtragem por região (cidade/estado)  
- Edição de dados das lojas  
- Cadastro de lojas (Administrador)
- Remoção de lojas (Administrador)  

---

## Perfis de Usuário

- **Perfil Consulta:** acesso às funcionalidades de consulta e edição 
- **Perfil Gestor:** acesso completo, incluindo cadastro e remoção de lojas  

---

## Estrutura do Projeto

```
src/
├── main.c / main.cpp
├── backend/
│ ├── lojas.c / lojas.h
│ ├── json.c
│ ├── mapa.c
│ ├── utils.c
│ └── cJSON.c / cJSON.h
├── ui/
│ ├── MainWindow.cpp / MainWindow.h
├── data/
│ ├── dados_exemplo.json
│ ├── dados_exemplo.csv
│ └── dados_backup-exemplo.json
└── resources/
├── logo.ico
├── mapa.html
├── resources.qrc
├── version.rc
├── icons/
└── imagens/
```
---

## Tecnologias Utilizadas

- C (lógica principal do sistema)
- C++
- Qt (interface gráfica em desenvolvimento/evolução)
- JSON (armazenamento de dados)
- HTML (mapa e visualização complementar)
- CMake (build do projeto)

---

### Download

[![Download](https://img.shields.io/badge/Download-Executável-brightgreen?style=for-the-badge&logo=download)](https://github.com/JulianaVBSantos/military-house-business-locator/releases/download/v1.2/Setup_Localizador_Comercial_Casa_Do_Militar_v1.2.exe)

---

## Recursos

O sistema inclui:

- Ícones personalizados
- Logo do sistema (logo.ico)
- Mapa em HTML integrado
- Interface Qt

---

## Sobre o projeto

Este sistema foi desenvolvido com foco em desempenho e simplicidade, considerando a realidade da instituição parceira. 

---

## Licença

Este projeto é de uso restrito. Consulte o arquivo LICENSE para mais informações.
