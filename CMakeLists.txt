cmake_minimum_required(VERSION 3.16)

project(LocalizadorComercialCasaDoMilitar LANGUAGES C CXX)

# Padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt AUTOMOC
set(CMAKE_AUTOMOC ON)

# Encontrar Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Recursos (.qrc)
qt6_add_resources(RESOURCES
    src/resources/resources.qrc
)

# Criar executável
add_executable(LocalizadorComercialCasaDoMilitar
    
    src/main.cpp # Main

    # UI
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h

    # Backend C
    src/backend/lojas.c
    src/backend/json.c
    src/backend/mapa.c
    src/backend/utils.c
    src/backend/cJSON.c

    # Recursos
    ${RESOURCES}
)

target_sources(LocalizadorComercialCasaDoMilitar PRIVATE
    src/resources/version.rc
)

# Linkar Qt
target_link_libraries(LocalizadorComercialCasaDoMilitar PRIVATE Qt6::Widgets)