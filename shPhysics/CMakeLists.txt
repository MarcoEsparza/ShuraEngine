project(shPhysics)
# Configure the shPhysics library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES}
)

# Include directories for the shPhysics library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link dependencies for the shPhysics library
target_link_libraries(${PROJECT_NAME} PUBLIC
    shUtilities
)
# Configure the debug suffix
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shPhysics target
set_output_directories(${PROJECT_NAME})