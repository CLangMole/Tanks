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
    explicit ResourceManager(const std::string& path);
    ~ResourceManager() = default;
#pragma region
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&&) = delete;
#pragma endregion

    std::shared_ptr<Renderer::ShaderProgram> load_shaders(const std::string& shaderName,
                                                          const std::string& vertexPath,
                                                          const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram> get_shader(const std::string& shaderName);

    std::shared_ptr<Renderer::Texture2D> load_texture(const std::string& name, const std::string& path);
    std::shared_ptr<Renderer::Texture2D> get_texture(const std::string& name);

    std::shared_ptr<Renderer::Sprite> load_sprite(const std::string& spriteName, const std::string& textureName, const std::string &shaderName,
                                                  const unsigned width, const unsigned height, const std::string& subTextureName = "default");
    std::shared_ptr<Renderer::Sprite> get_sprite(const std::string& name);

    std::shared_ptr<Renderer::AnimatedSprite> load_animatedSprite(const std::string& spriteName, const std::string& textureName, const std::string &shaderName,
                                                                  const unsigned width, const unsigned height, const std::string& subTextureName = "default");
    std::shared_ptr<Renderer::AnimatedSprite> get_animatedSprite(const std::string& name);

    std::shared_ptr<Renderer::Texture2D> load_textureAtlas(std::string textureName, std::string texturePath, std::vector<std::string> subTextures,
                                                           const unsigned int subTextureWidth, const unsigned int subTextureHeight);

private:
#pragma region
    typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> shaderProgramsMap;
    typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> texturesMap;
    typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> spritesMap;
    typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> animatedSpritesMap;
#pragma endregion

    std::string get_file_string(const std::string& relativeFilePath) const;

    shaderProgramsMap shaderPrograms;
    texturesMap textures;
    spritesMap sprites;
    animatedSpritesMap animatedSprites;

    std::string resourcePath;
};
