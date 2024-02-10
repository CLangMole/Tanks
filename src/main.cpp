#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include "Resources/ResourceManager.h"

using namespace std;

GLfloat texPositions[]{
        0.5f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
};

int main(int argc, char **argv) {
    if (!glfwInit()) {
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
    }

    glClearColor(0, 1, 1, 0);

    {
        ResourceManager resourceManager(argv[0]);

        auto spriteShaderProgram = resourceManager.load_shaders("DefaultSpriteShader", "res/shaders/vSprite.txt",
                                                                 "res/shaders/fSprite.txt");

        if (!spriteShaderProgram) {
            cerr << "Can't create a shader program " << "DefaultSpriteShader" << std::endl;
            return -1;
        }

        std::vector<std::string> subTexturesNames{
            "GreenTank0", "GreenTank1", "GreenTank2", "GreenTank3", "GreenTank4",
            "SingleBrick", "Bush", "Water", "SinglePlate", "Asphalt",
            "RedTank0", "RedTank1", "RedTank2", "RedTank3", "RedTank4",
            "DestroyedBrick", "Brick", "Plate", "Cup", "Box",
            "BlueTank0", "BlueTank1", "BlueTank2", "BlueTank3", "BlueTank4",
            "Heart", "HPBarLeft", "HPBarRight", "GreenLine", "RedLine",
            "SmallBullet0", "SmallBullet1", "SmallBullet2", "SmallBullet3", "SmallBullet4", "SmallBullet5",
            "GreenTankTower", "GreenTankWithoutTower0", "GreenTankWithoutTower1", "GreenTankWithoutTower2", "GreenTankWithoutTower3",
            "BigBullet0", "BigBullet1", "BigBullet2", "BigBullet3", "BigBullet4", "BigBullet5",
            "Empty1", "Empty2", "DestroyedGreenTank", "DestroyedRedTank", "DestroyedBlueTank"
        };

        auto textureAtlas = resourceManager.load_textureAtlas("DefaultTextureAtlas", "res/textures/TankTownTiles.png", subTexturesNames, 32, 32);

        auto sprite = resourceManager.load_sprite("DefaultSprite", "DefaultTextureAtlas", "DefaultSpriteShader", 100.0f, 100.0f, "Bush");
        sprite->set_position(glm::vec2(300, 100));

        auto animatedSprite = resourceManager.load_animatedSprite("DefaultAnimatedSprite", "DefaultTextureAtlas", "DefaultSpriteShader", 100.0f, 100.0f, "GreenTank0");
        animatedSprite->set_position(glm::vec2(300, 300));

        std::vector<std::pair<std::string, uint64_t>> greenTankState;
        greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank0", 1000000000));
        greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank1", 1000000000));
        greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank2", 1000000000));
        greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank3", 1000000000));
        greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank4", 1000000000));

        animatedSprite->add_state("GreenTank", std::move(greenTankState));
        animatedSprite->set_state("GreenTank");

        glm::mat4 projectionMatrix = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -100.0f, 100.0f);

        spriteShaderProgram->use();
        spriteShaderProgram->set_int("tex", 0);
        spriteShaderProgram->set_matrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow)) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t delta = chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            animatedSprite->update(delta);

            glClear(GL_COLOR_BUFFER_BIT);

            sprite->render();
            animatedSprite->render();

            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
