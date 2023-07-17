cmake_minimum_required (VERSION 3.20)
 
set(PROJECT_NAME MeatBeater)

project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

set(SRC_FILES 
main.cpp
InputComparators.cpp
BattleLevel.cpp
Stage1.cpp
Background.cpp
Character.cpp
Char1.cpp
FixedQueue.cpp
InputState.cpp
Actions.cpp
ActionResolver.cpp
Hit.cpp
)

set(INCLUDE_DIRS
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/Core
)

set(LINK_LIBRARIES
${sdl_LIBRARIES_DEBUG}
SDL2::SDL2
SDL2_image::SDL2_image
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

add_subdirectory (Core)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES} Core)