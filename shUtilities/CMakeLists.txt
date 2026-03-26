#project(shUtilities)

find_package(Lua REQUIRED)

# Configure the shUtilities library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(shUtilities SHARED ${SOURCE_FILES})

# Include directories for the shUtilities library
target_include_directories(shUtilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/externals
    ${LUA_INCLUDE_DIR}
)
# Link dependencies for the shUtilities library
target_link_libraries(shUtilities PUBLIC
    ${LUA_LIBRARIES}
)
# Configure the debug suffix
set_target_properties(shUtilities PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shUtilities target
set_output_directories(shUtilities)