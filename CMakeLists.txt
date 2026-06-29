cmake_minimum_required(VERSION 3.30)
project(Iron_Tide LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

# SDL
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.18)
else()
    message(STATUS "Using local SDL3")
endif()

# ENet
add_library(enet STATIC
        external/enet/enet.c
)

target_include_directories(enet PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/enet
)

if (WIN32)
    target_link_libraries(enet PUBLIC ws2_32 winmm)
endif()

# Shared
add_library(Iron_Tide_Shared INTERFACE)

target_include_directories(Iron_Tide_Shared INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shared
)

target_link_libraries(Iron_Tide_Shared INTERFACE
        enet
)

# Client
file(GLOB_RECURSE IRON_TIDE_CLIENT_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp
)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    list(APPEND IRON_TIDE_CLIENT_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    )
endif()

if (NOT IRON_TIDE_CLIENT_SOURCES)
    message(FATAL_ERROR
            "No client source files found in src/client or main.cpp"
    )
endif()

add_executable(Iron_Tide_Client WIN32
        ${IRON_TIDE_CLIENT_SOURCES}
)

target_compile_definitions(Iron_Tide_Client PRIVATE SDL_MAIN_USE_CALLBACKS)

target_include_directories(Iron_Tide_Client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Iron_Tide_Client PRIVATE
        Iron_Tide_Shared
        SDL3::SDL3
)

if (WIN32)
    add_custom_command(TARGET Iron_Tide_Client POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:Iron_Tide_Client>"
            VERBATIM
    )
endif()

# Server
file(GLOB_RECURSE IRON_TIDE_SERVER_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp
)

if (IRON_TIDE_SERVER_SOURCES)
    add_executable(Iron_Tide_Server
            ${IRON_TIDE_SERVER_SOURCES}
    )

    target_include_directories(Iron_Tide_Server PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/server
    )

    target_link_libraries(Iron_Tide_Server PRIVATE
            Iron_Tide_Shared
    )

    message(STATUS "Iron_Tide_Server target enabled (found server sources).")
else()
    message(STATUS "No server sources in src/server; Iron_Tide_Server target will NOT be built yet.")
endif()
