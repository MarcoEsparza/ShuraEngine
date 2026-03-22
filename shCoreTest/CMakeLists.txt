project(shCoreTest)
# Add the executable target for the shCoreTest module
add_executable(${PROJECT_NAME}
    source/imgui.cpp
    source/imgui_demo.cpp
    source/imgui_draw.cpp
    source/imgui_impl_shura.cpp
    source/imgui_tables.cpp
    source/imgui_widgets.cpp
    source/shCoreTestMain.cpp
    source/shGUI.cpp
    source/shRendererApp.cpp
)
# Include directories for the shCoreTest module
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/externals
)
# Add shCore as a dependency
target_link_libraries(${PROJECT_NAME} PRIVATE
    shCore
    shUtilities
    shPhysics
)
# Platform-specific libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        shDX11Graphics
        shOpenGLGraphics
    )
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        shOpenGLGraphics
    )
endif()
# Set the output directories for the shCoreTest target
set_output_directories(${PROJECT_NAME})