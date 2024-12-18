project(shCoreTest)
# Añadir el ejecutable de tests
add_executable(${PROJECT_NAME}
    source/shCoreTestMain.cpp
)
# Incluir los headers de lCore
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/shCore/include
    include
)
# Añadir lCore como dependencia
target_link_libraries(${PROJECT_NAME} PRIVATE shCore)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})