project(Monopoly-unit-tests)

set(TEST_SOURCE_FILES
        Game.cpp
        )

add_executable(Monopoly-tests ${TEST_SOURCE_FILES})

target_compile_definitions(Monopoly-tests PRIVATE MONOPOLY_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

target_link_libraries(Monopoly-tests gtest gtest_main Monopoly-lib)