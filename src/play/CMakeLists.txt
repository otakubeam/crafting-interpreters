# --------------------------------------------------------------------

add_executable(playground main.cpp)
target_link_libraries(playground PRIVATE Catch2::Catch2)
target_link_libraries(playground PUBLIC lex parse rt ast fmt::fmt)

# --------------------------------------------------------------------
