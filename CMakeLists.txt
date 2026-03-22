cmake_minimum_required(VERSION 3.20)
project(ShuraEngine)
# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(PLATFORM_TARGET Win64)
elseif(UNIX)
    set(PLATFORM_TARGET Linux)
endif()

# Platform-specific settings
add_compile_definitions(
    $<$<PLATFORM_ID:Windows>:PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Linux>:PLATFORM_LINUX>
)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_TARGET})
set(INTERMEDIATE_DIR ${CMAKE_SOURCE_DIR}/intermediate)
# Function to set output directories for targets
function(set_output_directories target_name)
    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${INTERMEDIATE_DIR}/${target_name}/${PLATFORM_TARGET}/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/$<CONFIG>"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/$<CONFIG>"
    )
endfunction()

# Add subdirectories for each module
add_subdirectory(shUtilities)
add_subdirectory(shPhysics)
add_subdirectory(shCore)

if(WIN32)
add_subdirectory(shDX11Graphics)
add_subdirectory(shOpenGLGraphics)
elseif(UNIX)
add_subdirectory(shOpenGLGraphics)
endif()

add_subdirectory(shCoreTest)