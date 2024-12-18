project(shUtilities)
# Configurar la biblioteca compartida (DLL)
add_library(${PROJECT_NAME} SHARED
    source/shBoxAAB.cpp
    source/shBoxOBB.cpp
    source/shCapsule.cpp
    source/shColor.cpp
    source/shLinearColor.cpp
    source/shMatrix4.cpp
    source/shPlane.cpp
    source/shPlatformMath.cpp
    source/shQuaternion.cpp
    source/shRect.cpp
    source/shSphere.cpp
    source/shVector2.cpp
    source/shVector2i.cpp
    source/shVector3.cpp
    source/shVector4.cpp
)

# Incluir los archivos en la carpeta include
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Configurar el sufijo 'd' para Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})