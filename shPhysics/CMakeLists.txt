#project(shPhysics)
# Configure the shPhysics library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(shPhysics SHARED ${SOURCE_FILES})

# Include directories for the shPhysics library
target_include_directories(shPhysics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link dependencies for the shPhysics library
target_link_libraries(shPhysics PUBLIC
    shUtilities
)
# Configure the debug suffix
set_target_properties(shPhysics PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shPhysics target
set_output_directories(shPhysics)