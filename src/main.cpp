#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Resources/ResourceManager.h"
#include "Game/Game.h"
#include <chrono>

using namespace std;

glm::ivec2 windowSize(640, 480);
Game game(windowSize);

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

    game.set_key(key, action);
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int height, int width){
    windowSize.y = height;
    windowSize.x = width;

    glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
    if (!glfwInit()) {
        cout << "Can't initialize glfw" << endl;
        return -1;
    }

    cout << argc << endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *pWindow = glfwCreateWindow(640, 480, "MyTest", nullptr, nullptr);

    if (!pWindow) {
        cout << "Failed to create a window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        cout << "Can't load glad" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);

    glClearColor(0, 0, 0, 1);

    {
        ResourceManager::set_executablePath(argv[0]);
        game.init();
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow)) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t delta = chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            game.update(delta);

            glClear(GL_COLOR_BUFFER_BIT);

            game.render();

            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }

        ResourceManager::unload_all();
    }

    glfwTerminate();
    return 0;
}
