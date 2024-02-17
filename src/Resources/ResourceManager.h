#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

class ResourceManager
{
public:

#pragma region
    ResourceManager() = delete;
    ~ResourceManager() = delete;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&&) = delete;
#pragma endregion

    static void set_executablePath(const std::string& path);
    static void unload_all();

    static std::shared_ptr<RenderEngine::ShaderProgram> load_shaders(const std::string& shaderName,
                                                                     const std::string& vertexPath,
                                                                     const std::string& fragmentPath);
    static std::shared_ptr<RenderEngine::ShaderProgram> get_shaderProgram(const std::string& shaderName);

    static std::shared_ptr<RenderEngine::Texture2D> load_texture(const std::string& name, const std::string& path);
    static std::shared_ptr<RenderEngine::Texture2D> get_texture(const std::string& name);

    static std::shared_ptr<RenderEngine::Sprite> load_sprite(const std::string& spriteName, const std::string& textureName, const std::string &shaderName,
                                                             const unsigned width, const unsigned height, const std::string& subTextureName = "default");
    static std::shared_ptr<RenderEngine::Sprite> get_sprite(const std::string& name);

    static std::shared_ptr<RenderEngine::AnimatedSprite> load_animatedSprite(const std::string& spriteName, const std::string& textureName, const std::string &shaderName,
                                                                             const unsigned width, const unsigned height, const std::string& subTextureName = "default");
    static std::shared_ptr<RenderEngine::AnimatedSprite> get_animatedSprite(const std::string& name);

    static std::shared_ptr<RenderEngine::Texture2D> load_textureAtlas(const std::string& textureName, const std::string& texturePath, const std::vector<std::string>& subTextures,
                                                                      const unsigned int subTextureWidth, const unsigned int subTextureHeight);

    static bool load_json(const std::string& path);

private:
#pragma region
    typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> shaderProgramsMap;
    typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> texturesMap;
    typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> spritesMap;
    typedef std::map<const std::string, std::shared_ptr<RenderEngine::AnimatedSprite>> animatedSpritesMap;
#pragma endregion

    static std::string get_file_string(const std::string& relativeFilePath);

    static shaderProgramsMap shaderPrograms;
    static texturesMap textures;
    static spritesMap sprites;
    static animatedSpritesMap animatedSprites;

    static std::string resourcePath;
};
