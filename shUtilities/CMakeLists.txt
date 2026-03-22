project(shUtilities)

find_package(Lua 5.4 REQUIRED)

# Configure the shUtilities library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES}
)

# Include directories for the shUtilities library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/externals
    ${LUA_INCLUDE_DIRS}
)
# Link dependencies for the shUtilities library
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LUA_LIBRARIES}
)
# Configure the debug suffix
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shUtilities target
set_output_directories(${PROJECT_NAME})