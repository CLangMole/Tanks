#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb_image.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::shaderProgramsMap ResourceManager::shaderPrograms;
ResourceManager::texturesMap ResourceManager::textures;
ResourceManager::spritesMap ResourceManager::sprites;
std::string ResourceManager::resourcePath;
std::vector<std::vector<std::string>> ResourceManager::levels;
std::vector<std::string> ResourceManager::startScreen;

void ResourceManager::set_executablePath(const std::string &path) {
    size_t found = path.find_last_of("/\\");
    resourcePath = path.substr(0, found);
}

void ResourceManager::unload_all() {
    shaderPrograms.clear();
    textures.clear();
    sprites.clear();
}

std::string ResourceManager::get_file_string(const std::string &relativeFilePath){
    std::ifstream in;
    in.open(resourcePath + "/" + relativeFilePath, std::ios::in | std::ios::binary);

    if (!in.is_open()){
        std::cerr << "Failed to open file " << relativeFilePath << std:: endl;
        return std::string {};
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::load_shaders(
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

    std::shared_ptr<RenderEngine::ShaderProgram>& shaderProgram = shaderPrograms
            .emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexShader, fragmentShader)).first->second;
    if (shaderProgram->is_compiled()){
        return shaderProgram;
    }

    std::cerr << "Can't load shaders:\n"
    << "Vertex: " << vertexPath << "\n"
    << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::get_shaderProgram(const std::string &shaderName) {
    auto it = shaderPrograms.find(shaderName);
    if (it != shaderPrograms.end()){
        return it->second;
    }

    std::cerr << "Can't find a shader program " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::load_texture(const std::string &name, const std::string &path) {
    int channels = 0;

    int width = 0;
    int height = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* pixels = stbi_load(std::string(resourcePath + "/" + path).c_str(), &width, &height, &channels, 0);

    if (!pixels){
        std::cerr << "Can't load a texture " << path << std::endl;
        return nullptr;
    }

    std::shared_ptr<RenderEngine::Texture2D> texture = textures.emplace(name
            , std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

    stbi_image_free(pixels);

    return texture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::get_texture(const std::string &name) {
    auto it = textures.find(name);
    if (it != textures.end()){
        return it->second;
    }

    std::cerr << "Can't find a texture " << name << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Sprite>
ResourceManager::load_sprite(const std::string &spriteName, const std::string &textureName,
                             const std::string &shaderName, const std::string &subTextureName) {

    auto texture = get_texture(textureName);
    if (!texture) {
        std::cerr << "Can't find a texture " << textureName << " for the aliveSprite " << spriteName << std::endl;
    }

    auto shader = get_shaderProgram(shaderName);
    if (!shader) {
        std::cerr << "Can't find a shader " << shaderName << " for the aliveSprite " << spriteName << std::endl;
    }

    std::shared_ptr<RenderEngine::Sprite> sprite = sprites.emplace(spriteName,
                                                                   std::make_shared<RenderEngine::Sprite>(texture,
                                                                                                          subTextureName,
                                                                                                          shader)).first->second;

    return sprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::get_sprite(const std::string &name) {
    auto it = sprites.find(name);
    if (it != sprites.end()){
        return it->second;
    }

    std::cerr << "Can't find a aliveSprite " << name << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::load_textureAtlas(const std::string& textureName, const std::string& texturePath, const std::vector<std::string>& subTextures,
                                                                            const unsigned int subTextureWidth, const unsigned int subTextureHeight) {

    auto texture = load_texture(textureName, texturePath);

    if (texture){
        const unsigned int textureWidth = texture->get_width();
        const unsigned int textureHeight = texture->get_height();

        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;

        for (auto& currentSubTextureName : subTextures) {
            glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01) / static_cast<float>(textureWidth),
                                   static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01) /
                                   static_cast<float>(textureHeight));
            glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01) /
                                 static_cast<float>(textureWidth),
                                 static_cast<float>(currentTextureOffsetY - 0.01) / static_cast<float>(textureHeight));

            texture->add_subTexture(currentSubTextureName, leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;

            if (currentTextureOffsetX >= textureWidth) {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureWidth;
            }
        }
    }

    return texture;
}

bool ResourceManager::load_json(const std::string &path) {

    const std::string JSONStr = get_file_string(path);

    if(JSONStr.empty()){
        std::cerr << "JSON file " << path << " is empty" << std::endl;
        return false;
    }

    rapidjson::Document document;
    rapidjson::ParseResult result = document.Parse(JSONStr.c_str());

    if(!result){
        std::cerr << "JSON parse error: %s (%u)" << GetParseError_En(result.Code()) << "(" << result.Offset()<< ")" << std::endl;
        std::cerr << "In JSON file" << path << std::endl;
        return false;
    }

    auto shadersIt = document.FindMember("shaders");
    if (shadersIt != document.MemberEnd()){
        for (const auto& currentShader : shadersIt->value.GetArray()){
            const std::string name = currentShader["name"].GetString();
            const std::string vPath = currentShader["vertexPath"].GetString();
            const std::string fPath = currentShader["fragmentPath"].GetString();

            load_shaders(name, vPath, fPath);
        }
    }

    auto textureAtlasesIt = document.FindMember("textureAtlases");
    if (textureAtlasesIt != document.MemberEnd()){
        for (const auto& currentAtlas : textureAtlasesIt->value.GetArray()){
            const std::string name = currentAtlas["name"].GetString();
            const std::string filePath = currentAtlas["path"].GetString();

            const auto subTexturesNamesArray = currentAtlas["subTextureNames"].GetArray();
            std::vector<std::string> subTexturesNamesVector;
            subTexturesNamesVector.reserve(subTexturesNamesArray.Size());

            for (const auto& currentSubTexture : subTexturesNamesArray){
                subTexturesNamesVector.emplace_back(currentSubTexture.GetString());
            }

            const unsigned int subTextureWidth = currentAtlas["subTextureWidth"].GetUint();
            const unsigned int subTextureHeight = currentAtlas["subTextureHeight"].GetUint();

            load_textureAtlas(name, filePath, subTexturesNamesVector, subTextureWidth, subTextureHeight);
        }
    }

    auto spritesIt = document.FindMember("sprites");
    if (spritesIt != document.MemberEnd()) {
        for (const auto &currentSprite: spritesIt->value.GetArray()) {
            const std::string spriteName = currentSprite["name"].GetString();
            const std::string textureAtlasName = currentSprite["textureAtlas"].GetString();
            const std::string shaderName = currentSprite["shader"].GetString();
            const std::string initialSubTextureName = currentSprite["initialSubTexture"].GetString();

            auto sprite = load_sprite(spriteName, textureAtlasName, shaderName, initialSubTextureName);
            if (!sprite) {
                continue;
            }

            auto framesIt = currentSprite.FindMember("frames");
            if (framesIt != currentSprite.MemberEnd()) {
                const auto framesArray = framesIt->value.GetArray();
                std::vector<RenderEngine::Sprite::FrameDescription> framesDescriptions;
                framesDescriptions.reserve(framesArray.Size());

                for (const auto &currentFrame: framesArray) {
                    const std::string subTextureName = currentFrame["subTexture"].GetString();
                    const double duration = currentFrame["duration"].GetDouble();

                    const auto textureAtlas = get_texture(textureAtlasName);
                    const auto subTexture = textureAtlas->get_subTexture(subTextureName);

                    framesDescriptions.emplace_back(subTexture.leftBottomUV, subTexture.rightTopUV, duration);
                }

                sprite->addFrames(std::move(framesDescriptions));
            }
        }
    }

    auto startScreenIt = document.FindMember("startScreen");
    if (startScreenIt != document.MemberEnd()) {
        const auto descriptionsArray = startScreenIt->value.GetArray();
        startScreen.reserve(descriptionsArray.Size());
        size_t maxLength = 0;

        for (const auto &currentRow: descriptionsArray) {
            startScreen.emplace_back(currentRow.GetString());
            if (maxLength < startScreen.back().length()) {
                maxLength = startScreen.back().length();
            }
        }

        for (auto &currentRow: startScreen) {
            while (currentRow.length() < maxLength) {
                currentRow.append("F");
            }
        }
    }

    auto levelsIt = document.FindMember("levels");
    if (levelsIt != document.MemberEnd()) {
        for (const auto &currentLevel: levelsIt->value.GetArray()) {
            const auto descriptionsArray = currentLevel["description"].GetArray();
            std::vector<std::string> levelRows;
            levelRows.reserve(descriptionsArray.Size());
            size_t maxLength = 0;

            for (const auto &currentRow: descriptionsArray) {
                levelRows.emplace_back(currentRow.GetString());
                if (maxLength < levelRows.back().length()) {
                    maxLength = levelRows.back().length();
                }
            }

            for (auto &currentRow: levelRows) {
                while (currentRow.length() < maxLength) {
                    currentRow.append("D");
                }
            }

            levels.emplace_back(std::move(levelRows));
        }
    }

    return true;
}
