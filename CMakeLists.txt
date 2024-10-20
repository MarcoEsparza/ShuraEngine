cmake_minimum_required(VERSION 3.20)
project(ShuraEngine)
# Configuración global
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Detectar el compilador y configurar las advertencias según corresponda
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Para GCC o Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Para MSVC (Microsoft Visual Studio)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()
# Plataforma y configuración
set(PLATFORM_TARGET $<IF:$<PLATFORM_ID:Windows>,Win64,Linux>)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_TARGET})
set(INTERMEDIATE_DIR ${CMAKE_SOURCE_DIR}/intermediate)
# Función para establecer las rutas de salida
function(set_output_directories target_name)
    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${INTERMEDIATE_DIR}/${target_name}/${PLATFORM_TARGET}/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    )
endfunction()
# Añadir subdirectorios
add_subdirectory(shUtilities)
add_subdirectory(shUtilitiesTest)