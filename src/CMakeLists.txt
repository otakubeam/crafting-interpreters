project(interpreter)

# --------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

# --------------------------------------------------------------------

function(add_task_library DIR_NAME)

    set(LIB_TARGET ${DIR_NAME})
    set(LIB_DIR ${DIR_NAME})

    file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_DIR}/*.cpp)
    file(GLOB_RECURSE LIB_HEADERS ${LIB_DIR}/*.hpp ${LIB_DIR}/*.ipp)
    add_library(${LIB_TARGET} STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})

    # Include dir
    get_filename_component(LIB_INCLUDE_DIR "${LIB_DIR}/.." ABSOLUTE)
    target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_DIR})

endfunction()

# --------------------------------------------------------------------

add_task_library(lex)

add_task_library(parse)

# --------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

function(add_task_executable BINARY_NAME)

    set(BINARY_SOURCES ${ARGN})

    add_executable(${BINARY_NAME} ${BINARY_SOURCES} ${TASK_SOURCES})
    target_link_libraries(${BINARY_NAME} lex parse)

endfunction()

# --------------------------------------------------------------------

function(add_task_test_dir DIR_NAME)

    set(BINARY_NAME ${DIR_NAME})

    set(TEST_DIR "${DIR_NAME}/test")
    file(GLOB_RECURSE TEST_CXX_SOURCES ${TEST_DIR}/*.cpp)

    set(TEST_NAME "${DIR_NAME}_test")
    add_task_executable(${TEST_NAME} ${TEST_CXX_SOURCES})

endfunction()

# --------------------------------------------------------------------

add_task_test_dir(parse)

