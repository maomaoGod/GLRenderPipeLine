cmake_minimum_required(VERSION 3.19)
project(GLRenderPipeLine)

set(CMAKE_CXX_STANDARD 14)
set(ASSIMP_HEADER /opt/homebrew/Cellar/assimp/5.0.1/include/)
include_directories("header" ${ASSIMP_HEADER})
include_directories("src")

set(GLEW_LINK /opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.4/lib/libglfw.3.3.dylib)
set(ASSIMP_LINK /opt/homebrew/Cellar/assimp/5.0.1/lib/libassimp.5.0.0.dylib)
#set(LRRXML_LINK /opt/homebrew/Cellar/assimp/5.0.1/lib/liblrrXML.dylib)
set(Libs ${GLEW_LINK} ${GLFW_LINK} ${ASSIMP_LINK})
link_libraries(${GLRenderPipeLine} ${GLEW_LINK} ${GLFW_LINK} ${ASSIMP_LINK})

set(SOURCE_FILES  "src/glad.c" "src/stb_image.cpp" "src/imgui/imgui.cpp" "src/imgui/imgui_draw.cpp" "src/imgui/imgui_demo.cpp" "src/imgui/imgui_impl_glfw.cpp" "src/imgui/imgui_impl_opengl3.cpp" "src/imgui/imgui_tables.cpp" "src/imgui/imgui_widgets.cpp" "main.cpp" header/Rendering/IRenderSystem.h header/Scene.h header/SceneManager.h header/TSingleton.h header/entity/primitive/skybox.h header/MeshManager.h header/mesh/cubemesh.h header/mesh/planemesh.h header/material/material.h header/texture/texture.h header/Rendering/ForwardRenderSystem.h header/entity/transform.h header/mesh/quadmesh.h header/entity/primitive/quad.h header/entity/primitive/screenrenderquad.h header/postprocess/postprocessmgr.h header/entity/light.h)
add_executable(GLRenderPipeLine ${SOURCE_FILES})

if(APPLE)
    target_link_libraries(GLRenderPipeLine "-framework OpenGL")
    target_link_libraries(GLRenderPipeLine "-framework GLUT")
    target_link_libraries(GLRenderPipeLine ${Libs})
endif()
