project(shOpenGLGraphics)
# Configure the shOpenGLGraphics library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES}
)

# Include directories for the shOpenGLGraphics library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link dependencies for the shOpenGLGraphics library
target_link_libraries(${PROJECT_NAME} PUBLIC
    shCore
    shUtilities
)
# Configure the debug suffix
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shOpenGLGraphics target
set_output_directories(${PROJECT_NAME})