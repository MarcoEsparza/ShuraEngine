project(shOpenGLGraphics)
# Configurar la biblioteca compartida (DLL)
add_library(${PROJECT_NAME} SHARED
    source/shOGLGraphicsManager.cpp
    source/shOGLLoadPlugin.cpp
)

# Incluir los archivos en la carpeta include
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Configurar el sufijo 'd' para Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})