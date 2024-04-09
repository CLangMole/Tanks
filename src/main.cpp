#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Resources/ResourceManager.h"
#include "Game/Game.h"
#include "Renderer/Renderer.h"
#include "Physics/PhysicsEngine.h"

#include <chrono>

using namespace std;

static constexpr unsigned int SCALE = 3;
static constexpr unsigned int BLOCK_SIZE = 16;

glm::uvec2 windowSize(SCALE * 16 * BLOCK_SIZE, SCALE * 15 * BLOCK_SIZE);
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

    game->set_windowSize(windowSize);
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

    GLFWwindow *window = glfwCreateWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), "Tanks",
                                          nullptr, nullptr);

    if (!window) {
        cout << "Failed to create a window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        cout << "Can't load glad" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);

    RenderEngine::Renderer::set_clearColor(0, 0, 0, 1);
    RenderEngine::Renderer::set_depthTest(true);

    {
        ResourceManager::set_executablePath(argv[0]);
        Physics::PhysicsEngine::init();
        game->init();

//        glfwSetWindowSize(window, static_cast<int>(3 * game->get_stateWidth()),
//                          static_cast<int>(3 * game->get_stateHeight()));
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            auto currentTime = std::chrono::high_resolution_clock::now();
            double delta = chrono::duration<double, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;

            game->update(delta);
            Physics::PhysicsEngine::update(delta);

            RenderEngine::Renderer::clear();

            game->render();

            glfwSwapBuffers(window);
        }
        game = nullptr;
        ResourceManager::unload_all();
    }

    glfwTerminate();
    return 0;
}
