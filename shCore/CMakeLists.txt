#project(shCore)

find_package(assimp REQUIRED)

# Configure the shCore library target
file(GLOB SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

if(WIN32)
    file(GLOB PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/*Win32.cpp
    )
elseif(UNIX)
    file(GLOB PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/*X11.cpp
    )
endif()

add_library(shCore SHARED
    ${SOURCE_FILES}
    ${PLATFORM_SOURCES}
)

# Include directories for the shCore library
target_include_directories(shCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/externals
    include
)
# Link dependencies for the shCore library
target_link_libraries(shCore PUBLIC
    shUtilities
    shPhysics
    assimp::assimp
)
# Configure the debug suffix
set_target_properties(shCore PROPERTIES
    DEBUG_POSTFIX "d"
)
# Set the output directories for the shCore target
set_output_directories(shCore)