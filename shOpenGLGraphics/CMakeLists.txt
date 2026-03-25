#project(shOpenGLGraphics)
# Configure the shOpenGLGraphics library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(shOpenGLGraphics SHARED ${SOURCE_FILES})

# Include directories for the shOpenGLGraphics library
target_include_directories(shOpenGLGraphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link dependencies for the shOpenGLGraphics library
target_link_libraries(shOpenGLGraphics PUBLIC
    shCore
    shUtilities
)
# Configure the debug suffix
set_target_properties(shOpenGLGraphics PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shOpenGLGraphics target
set_output_directories(shOpenGLGraphics)