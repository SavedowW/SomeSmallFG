cmake_minimum_required (VERSION 3.20)
 
project(Core)

set(CORE_SRC_FILES
Timer.cpp
InputSystem.cpp
TextureManager.cpp
Camera.cpp
Renderer.cpp
Window.cpp
Renderer.cpp
Application.cpp
Level.cpp
AnimationManager.cpp
FrameTimer.cpp
HUD.cpp
TextManager.cpp
ParticleManager.cpp
TimelineProperty.cpp
ResourceCounter.cpp
StateMarker.cpp
)

add_library (Core ${CORE_SRC_FILES})

set_property(TARGET Core PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(Core ${LINK_LIBRARIES})