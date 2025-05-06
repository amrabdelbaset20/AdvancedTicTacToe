# Create object library for game components
add_library(game_objects OBJECT
    ../src/game/game.cpp
    ../src/game/board.cpp
    ../src/game/player.cpp
    ../src/game/ai.cpp
    ../src/game/humanplayer.cpp
    # Note: gametypes.cpp is intentionally omitted
)

target_include_directories(game_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/game
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

target_link_libraries(game_objects PRIVATE
    Qt6::Core
)

# Create test executable
add_executable(
    game_tests
    test_game.cpp
    test_board.cpp
    test_ai.cpp
    $<TARGET_OBJECTS:game_objects>
)

target_link_libraries(
    game_tests
    PRIVATE
    gtest_main
    gmock_main
    Qt6::Core
)

target_include_directories(game_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/game
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${googletest_SOURCE_DIR}/googletest/include
    ${googletest_SOURCE_DIR}/googlemock/include
)

add_test(
    NAME game_tests
    COMMAND game_tests
)