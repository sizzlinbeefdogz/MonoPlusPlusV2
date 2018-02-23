cmake_minimum_required(VERSION 3.8)
project(cplushoarding)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Gamestate.cpp Gamestate.h Rules.cpp Rules.h Space.cpp Space.h Board.cpp Board.h Property.cpp Property.h Go.cpp Go.h Player.cpp Player.h Move.cpp Move.h DiceRoller.cpp DiceRoller.h Upgrade.cpp Upgrade.h Auction.cpp Auction.h)
add_executable(cplushoarding ${SOURCE_FILES})