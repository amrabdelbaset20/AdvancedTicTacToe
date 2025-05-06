cmake_minimum_required(VERSION 3.15)
project(AdvancedTicTacToe LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCE_FILES
    src/main.cpp
    src/game/game.cpp
    src/game/board.cpp
    src/game/player.cpp
    src/game/ai.cpp
    src/game/humanplayer.cpp
    src/game/gametypes.h
    src/gui/mainwindow.cpp
    src/gui/loginwindow.cpp
    src/gui/gamewindow.cpp
)

set(HEADER_FILES
    src/game/game.h
    src/game/board.h
    src/game/player.h
    src/game/ai.h
    src/game/humanplayer.h
    src/gui/mainwindow.h
    src/gui/loginwindow.h
    src/gui/gamewindow.h
)

qt_add_executable(AdvancedTicTacToe
    ${SOURCE_FILES}
    ${HEADER_FILES}
    resources/resources.qrc
)

target_include_directories(AdvancedTicTacToe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
)

target_link_libraries(AdvancedTicTacToe PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)