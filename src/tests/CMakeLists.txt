# Required for testing
find_package(Catch2 2 REQUIRED)

# --------------------------------------------------------------------

function(add_task_executable BINARY_NAME)

    set(BINARY_SOURCES ${ARGN})

    add_executable(${BINARY_NAME} ${BINARY_SOURCES} ${TASK_SOURCES})
    target_link_libraries(${BINARY_NAME} PUBLIC lex parse rt ast fmt::fmt)
    target_link_libraries(${BINARY_NAME} PRIVATE Catch2::Catch2)

endfunction()

# --------------------------------------------------------------------

function(add_task_test_dir DIR_NAME)

    set(BINARY_NAME ${DIR_NAME})

    file(GLOB_RECURSE TEST_CXX_SOURCES ${DIR_NAME}/*.cpp)

    set(TEST_NAME "${DIR_NAME}_test")
    add_task_executable(${TEST_NAME} ${TEST_CXX_SOURCES})

endfunction()

# --------------------------------------------------------------------

add_task_test_dir(lex)

add_task_test_dir(rt)

add_task_test_dir(parse)

add_task_test_dir(ast)

add_task_test_dir(play)

# --------------------------------------------------------------------
