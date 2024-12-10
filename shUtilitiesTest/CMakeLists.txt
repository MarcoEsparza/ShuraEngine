project(shUtilitiesTests)
# Añadir el ejecutable de tests
add_executable(${PROJECT_NAME}
    source/shUtilitiesTestMain.cpp
    include/catch.hpp
)
# Incluir los headers de lUtilities
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/shUtilities/include
    include
)
# Añadir lUtilities como dependencia
target_link_libraries(${PROJECT_NAME} PRIVATE shUtilities)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})