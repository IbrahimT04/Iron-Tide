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

find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.18)
else()
    message(STATUS "Using local SDL3")
endif()

# Name the target the same as your CLion run configuration if you want
add_executable(Iron_Tide WIN32
        main.cpp
)

# SDL3::SDL3 already carries include dirs, so this is enough:
target_compile_definitions(Iron_Tide PRIVATE SDL_MAIN_USE_CALLBACKS)
target_link_libraries(Iron_Tide PRIVATE SDL3::SDL3)

if (WIN32)
    add_custom_command(TARGET Iron_Tide POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:Iron_Tide>"
            VERBATIM)
endif()
