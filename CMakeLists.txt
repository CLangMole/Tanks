cmake_minimum_required(VERSION 3.21)
project(Tanks)

set(CMAKE_CXX_STANDARD 23)

add_executable(Tanks src/main.cpp
        src/Renderer/ShaderProgram.cpp src/Renderer/ShaderProgram.h
        src/Resources/ResourceManager.cpp src/Resources/ResourceManager.h
        src/Resources/stb_image.h
        src/Renderer/Texture2D.cpp src/Renderer/Texture2D.h
        src/Renderer/Sprite.cpp src/Renderer/Sprite.h
        src/Renderer/AnimatedSprite.cpp src/Renderer/AnimatedSprite.h
        src/Game/Game.cpp src/Game/Game.h src/Game/Tank.cpp src/Game/Tank.h src/Renderer/VertexBuffer.cpp src/Renderer/VertexBuffer.h src/Renderer/IndexBuffer.cpp src/Renderer/IndexBuffer.h src/Renderer/VertexArray.cpp src/Renderer/VertexArray.h src/Renderer/VertexBufferLayout.cpp src/Renderer/VertexBufferLayout.h src/Renderer/Renderer.cpp src/Renderer/Renderer.h)

add_subdirectory(external/glfw-3.3.9)
target_link_libraries(Tanks glfw)
target_include_directories(Tanks PUBLIC "${CMAKE_SOURCE_DIR}/external/glfw-3.3.9/include")

add_subdirectory(external/glad)
target_link_libraries(Tanks glad)
target_include_directories(Tanks PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/include")

include_directories(external/glm)
include_directories(external/RapidJSON/include)

set_target_properties(Tanks PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET Tanks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:Tanks>/res)