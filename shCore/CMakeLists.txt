project(shCore)
# Configurar la biblioteca compartida (DLL)
add_library(${PROJECT_NAME} SHARED
    source/shAnimatorComponent.cpp
    source/shBaseApp.cpp
    source/shCamera.cpp
    source/shDynamicLibrary.cpp
    source/shEvent.cpp
    source/shGameObject.cpp
    source/shGraphicsManager.cpp
    source/shMeshComponent.cpp
    source/shResourceManager.cpp
    source/shSceneGraph.cpp
    source/shScreenEventHandleWin32.cpp
    source/shScreenWin32.cpp
    source/shTransformComponent.cpp
)

# Incluir los archivos en la carpeta include
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Configurar el sufijo 'd' para Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})