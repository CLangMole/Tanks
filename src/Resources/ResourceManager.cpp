#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb_image.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::shaderProgramsMap ResourceManager::shaderPrograms;
ResourceManager::texturesMap ResourceManager::textures;
ResourceManager::spritesMap ResourceManager::sprites;
ResourceManager::animatedSpritesMap ResourceManager::animatedSprites;

void ResourceManager::set_executablePath(const std::string &path) {
    size_t found = path.find_last_of("/\\");
    resourcePath = path.substr(0, found);
}

void ResourceManager::unload_all() {
    shaderPrograms.clear();
    textures.clear();
    sprites.clear();
    animatedSprites.clear();
}

std::string ResourceManager::get_file_string(const std::string &relativeFilePath){
    std::ifstream in;
    in.open(resourcePath + "/" + relativeFilePath, std::ios::in | std::ios::binary);

    if (!in.is_open()){
        std::cerr << "Failed to open file" << relativeFilePath << std:: endl;
        return {};
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::load_shaders(
        const std::string &shaderName,
        const std::string &vertexPath,
        const std::string &fragmentPath) {

    std::string vertexShader = get_file_string(vertexPath);
    if (vertexShader.empty()){
        std::cerr << "No vertex shader in file " << vertexPath << std::endl;
        return nullptr;
    }

    std::string fragmentShader = get_file_string(fragmentPath);
    if (vertexShader.empty()){
        std::cerr << "No fragment shader in file " << fragmentPath << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram>& shaderProgram = shaderPrograms
            .emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexShader, fragmentShader)).first->second;
    if (shaderProgram->is_compiled()){
        return shaderProgram;
    }

    std::cerr << "Can't load_shaders shader program:\n"
    << "Vertex: " << vertexPath << "\n"
    << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::get_shader(const std::string &shaderName) {
    auto it = shaderPrograms.find(shaderName);
    if (it != shaderPrograms.end()){
        return it->second;
    }

    std::cerr << "Can't find a shader program " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::load_texture(const std::string &name, const std::string &path) {
    int channels = 0;

    int width = 0;
    int height = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* pixels = stbi_load(std::string(resourcePath + "/" + path).c_str(), &width, &height, &channels, 0);

    if (!pixels){
        std::cerr << "Can't load a texture " << path << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::Texture2D> texture = textures.emplace(name
            , std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

    stbi_image_free(pixels);

    return texture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::get_texture(const std::string &name) {
    auto it = textures.find(name);
    if (it != textures.end()){
        return it->second;
    }

    std::cerr << "Can't find a texture " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::load_sprite(const std::string &spriteName, const std::string &textureName,
                                                               const std::string &shaderName, const unsigned int width, const unsigned int height, const std::string& subTextureName) {

    auto texture = get_texture(textureName);
    if (!texture){
        std::cerr << "Can't find a texture " << textureName << " for the sprite " << spriteName << std::endl;
    }

    auto shader = get_shader(shaderName);
    if (!shader){
        std::cerr << "Can't find a shader " << shaderName << " for the sprite " << spriteName << std::endl;
    }

    std::shared_ptr<Renderer::Sprite> sprite = sprites.emplace(spriteName
            , std::make_shared<Renderer::Sprite>(texture, subTextureName, shader, glm::vec2(0.0f, 0.0f), glm::vec2(width, height))).first->second;

    return sprite;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::get_sprite(const std::string &name) {
    auto it = sprites.find(name);
    if (it != sprites.end()){
        return it->second;
    }

    std::cerr << "Can't find a sprite " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::load_animatedSprite(const std::string &spriteName, const std::string &textureName,
                                                                               const std::string &shaderName, const unsigned int width, const unsigned int height,
                                                                               const std::string &subTextureName) {
    auto texture = get_texture(textureName);
    if (!texture){
        std::cerr << "Can't find a texture " << textureName << " for the sprite " << spriteName << std::endl;
    }

    auto shader = get_shader(shaderName);
    if (!shader){
        std::cerr << "Can't find a shader " << shaderName << " for the sprite " << spriteName << std::endl;
    }

    std::shared_ptr<Renderer::AnimatedSprite> sprite = animatedSprites.emplace(spriteName
            , std::make_shared<Renderer::AnimatedSprite>(texture, subTextureName, shader, glm::vec2(0.0f, 0.0f), glm::vec2(width, height))).first->second;

    return sprite;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::get_animatedSprite(const std::string &name) {
    auto it = animatedSprites.find(name);
    if (it != animatedSprites.end()){
        return it->second;
    }

    std::cerr << "Can't find animated sprite " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::load_textureAtlas(const std::string& textureName, const std::string& texturePath, const std::vector<std::string>& subTextures,
                                   const unsigned int subTextureWidth, const unsigned int subTextureHeight) {

    auto texture = load_texture(textureName, texturePath);

    if (texture){
        const unsigned int textureWidth = texture->get_width();
        const unsigned int textureHeight = texture->get_height();

        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;

        for (auto& currentSubTextureName : subTextures){
            glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
            glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

            texture->add_subTexture(currentSubTextureName, leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;

            if (currentTextureOffsetX >= textureWidth){
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureWidth;
            }
        }
    }

    return texture;
}