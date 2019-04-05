cmake_minimum_required(VERSION 3.12)
project(Parcheesi)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

if (WIN32)
    # come back to this if it becomes necessary
elseif (APPLE)
    set(CMAKE_PREFIX_PATH "/Users/$ENV{USER}/Qt/5.12.1/clang_64")
endif ()
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


find_package(Qt5Core COMPONENTS REQUIRED)
find_package(Qt5Gui COMPONENTS REQUIRED)
find_package(Qt5Widgets COMPONENTS REQUIRED)

add_executable(Parcheesi main.cpp MainWindow.cpp MainWindow.h Board.cpp Board.h Tile.cpp Tile.h Die.cpp Die.h Player.cpp Player.h Pawn.cpp Pawn.h)

target_link_libraries(Parcheesi Qt5::Core Qt5::Gui Qt5::Widgets)
