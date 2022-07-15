project(interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

function(add_task_library DIR_NAME)
    set(LIB_TARGET lex)
    set(LIB_DIR .)

    file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_DIR}/*.cpp)
    file(GLOB_RECURSE LIB_HEADERS ${LIB_DIR}/*.hpp ${LIB_DIR}/*.ipp)
    add_library(${LIB_TARGET} STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})

    # Include dir
    get_filename_component(LIB_INCLUDE_DIR "${LIB_DIR}/.." ABSOLUTE)
    target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_DIR})

endfunction()

add_task_library(one)
