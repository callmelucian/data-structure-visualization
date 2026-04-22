cmake_minimum_required(VERSION 3.15)
project(DataStructuresVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/Program Files/SFML/SFML-3.0.2/lib/cmake/SFML")

# Change the number to the actual version of your SFML download
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics)

# 4. Include your own 'include' folder
include_directories(include)

# 5. Create the executable from your source files
add_executable(DataVisApp
    src/main.cpp
    
    src/ds/avl-tree.cpp
    src/ds/dijkstra.cpp
    src/ds/red-black-tree.cpp
    src/ds/bellman-ford.cpp
    src/ds/hash-map.cpp

    src/components/binary-tree.cpp
    src/components/button.cpp
    src/components/input-field.cpp
    src/components/nodes.cpp
    src/components/slider.cpp
    src/components/highlight-circle.cpp
    src/components/code-highlighter.cpp
    src/components/graph.cpp
    src/components/hash-map.cpp

    src/core/utility.cpp

    src/animation/animation-manager.cpp
    src/animation/events.cpp

    src/scenes/avl-tree-scene.cpp
    src/scenes/scene-manager.cpp
    src/scenes/dijkstra-scene.cpp
    src/scenes/prim-scene.cpp
    src/scenes/rb-tree-scene.cpp
    src/scenes/hash-map-scene.cpp
)

# 6. Link SFML to your app
target_link_libraries(DataVisApp SFML::Graphics)

if(WIN32)
    add_custom_command(TARGET DataVisApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:DataVisApp>
        $<TARGET_FILE_DIR:DataVisApp>
        COMMAND_EXPAND_LISTS)
endif()