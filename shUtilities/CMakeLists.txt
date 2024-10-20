project(shUtilities)
# Configurar la biblioteca compartida (DLL)
add_library(${PROJECT_NAME} SHARED
    Source/shBoxAAB.cpp
    Source/shBoxOBB.cpp
    Source/shCapsule.cpp
    Source/shMatrix4.cpp
    Source/shPlane.cpp
    Source/shPlatformMath.cpp
    Source/shQuaternion.cpp
    Source/shRect.cpp
    Source/shSphere.cpp
    Source/shVector2.cpp
    Source/shVector2i.cpp
    Source/shVector3.cpp
    Source/shVector4.cpp
)

# Incluir los archivos en la carpeta include
target_include_directories(${PROJECT_NAME} PUBLIC Include)
# Configurar el sufijo 'd' para Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})