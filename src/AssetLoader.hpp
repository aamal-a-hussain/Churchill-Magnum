#ifndef ANIMATION_LOADER_HPP
#define ANIMATION_LOADER_HPP

#include "Magnum/GL/GL.h"
#include <Magnum/GL/Texture.h>
#include <format>
#include <map>

enum class TextureType { Tile };
enum class AnimationType { Player };
enum class SoundType { Player };

namespace Resources {
class AssetLoader {

  AssetLoader() {}
  std::map<TextureType, Magnum::GL::Texture2D> m_textures;
  std::map<AnimationType, Magnum::GL::Texture2D> m_animation;

public:
  static AssetLoader &Instance() {
    static AssetLoader s_instance{};
    return s_instance;
  }

  Magnum::GL::Texture2D &GetTexture(TextureType type) {
    if (auto result = m_textures.find(type); result != m_textures.end())
      return result->second;
    throw std::runtime_error(
        std::format("Could not find texture of type {}", (int)type).c_str());
  }
};
} // namespace Resources

#endif // !ANIMATION_LOADER_HPP
