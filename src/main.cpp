#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Resources/ResourceManager.h"
#include "Game/Game.h"
#include "Renderer/Renderer.h"

#include <chrono>

using namespace std;

glm::ivec2 windowSize(13 * 16, 14 * 16);
std::unique_ptr<Game> game = std::make_unique<Game>(windowSize);

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scanCode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

    game->set_key(key, action);
}

void glfwWindowSizeCallback(GLFWwindow *pWindow, int height, int width) {
    windowSize.y = height;
    windowSize.x = width;

    const float aspectRatio = 13.0f / 14.0f;

    unsigned int viewportWidth = windowSize.x;
    unsigned int viewportHeight = windowSize.y;

    unsigned int leftOffset = 0;
    unsigned int bottomOffset = 0;

    if (static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y) > aspectRatio) {
        viewportWidth = static_cast<unsigned int>(static_cast<float>(windowSize.y) * aspectRatio);
        leftOffset = (windowSize.x - viewportWidth) / 2;
    } else {
        viewportHeight = static_cast<unsigned int>(static_cast<float>(windowSize.x) / aspectRatio);
        bottomOffset = (windowSize.y - viewportHeight) / 2;
    }
    RenderEngine::Renderer::set_viewport(viewportWidth, viewportHeight, leftOffset, bottomOffset);
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

    GLFWwindow *pWindow = glfwCreateWindow(windowSize.x, windowSize.y, "Tanks", nullptr, nullptr);

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

    RenderEngine::Renderer::set_clearColor(0, 0, 0, 1);

    {
        ResourceManager::set_executablePath(argv[0]);
        game->init();
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow)) {
            glfwPollEvents();
            auto currentTime = std::chrono::high_resolution_clock::now();
            double delta = chrono::duration<double, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;

            game->update(delta);

            RenderEngine::Renderer::clear();

            game->render();

            glfwSwapBuffers(pWindow);
        }
        game = nullptr;
        ResourceManager::unload_all();
    }

    glfwTerminate();
    return 0;
}
