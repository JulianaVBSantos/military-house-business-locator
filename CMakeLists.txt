cmake_minimum_required(VERSION 3.16)

project(MeuSistema LANGUAGES C CXX)

# Padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt AUTOMOC
set(CMAKE_AUTOMOC ON)

# Encontrar Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Criar executável
add_executable(MeuSistema

    # Main
    src/main.cpp

    # UI
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h

    # Backend C
    src/backend/lojas.c
    src/backend/json.c
    src/backend/mapa.c
    src/backend/utils.c
    src/backend/cJSON.c
)

# Linkar Qt
target_link_libraries(MeuSistema PRIVATE Qt6::Widgets)