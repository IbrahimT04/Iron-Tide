cmake_minimum_required(VERSION 3.30)
project(Iron_Tide LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

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

# ======================
# SDL3
# ======================
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.18)
else()
    message(STATUS "Using local SDL3")
endif()

# ======================
# ENet – built as our own static library
# ======================

# This library is just enet.c (which #defines ENET_IMPLEMENTATION and includes enet.h)
add_library(enet STATIC
        external/enet/enet.c
)

# Public headers for ENet (so you can #include <enet.h> from your code)
target_include_directories(enet PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/enet
)

# ENet depends on WinSock + winmm on Windows, like the usual ENet builds.
if (WIN32)
    target_link_libraries(enet PUBLIC ws2_32 winmm)
endif()

# ======================
# Your executable
# ======================

add_executable(Iron_Tide WIN32
        main.cpp
        car.h
        Car.cpp
)

# SDL "callbacks" main
target_compile_definitions(Iron_Tide PRIVATE SDL_MAIN_USE_CALLBACKS)

# Link SDL3 and ENet library target
target_link_libraries(Iron_Tide PRIVATE
        SDL3::SDL3
        enet
)

# Copy SDL3 DLL next to the exe on Windows (your existing code)
if (WIN32)
    add_custom_command(TARGET Iron_Tide POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:Iron_Tide>"
            VERBATIM
    )
endif()
