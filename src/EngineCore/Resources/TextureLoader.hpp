#ifndef INCLUDE_RESOURCES_TEXTURELOADER_HPP_
#define INCLUDE_RESOURCES_TEXTURELOADER_HPP_

#include "Corrade/Containers/Optional.h"
#include "Corrade/Containers/Pointer.h"
#include "Corrade/PluginManager/Manager.h"
#include "Corrade/Utility/Debug.h"
#include "Corrade/Utility/DebugAssert.h"
#include "Corrade/Utility/Resource.h"
#include "Magnum/GL/Sampler.h"
#include "Magnum/GL/Texture.h"
#include "Magnum/GL/TextureFormat.h"
#include "Magnum/ImageView.h"
#include "Magnum/Magnum.h"
#include "Magnum/Trade/AbstractImporter.h"
#include "Magnum/Trade/ImageData.h"
#include "Magnum/Trade/Trade.h"

#include <cstddef>
#include <unordered_map>
class TextureLoader {
public:
  enum class TextureId { IDLE, ATTACK, RUN, WALK };
  struct TextureInfo {
    Magnum::Vector2i sheetDimensions;
    std::size_t n_frames;
  };

  TextureLoader() {

    Corrade::Utility::Resource rs{"player-data"};
    _importTexture(rs, "player_idle.png", TextureId::IDLE, {{768, 128}, 6});
    CORRADE_INTERNAL_ASSERT(m_texture_map.contains(TextureId::IDLE));
    CORRADE_INTERNAL_ASSERT(m_texture_info_map.contains(TextureId::IDLE));
    _importTexture(rs, "player_attack.png", TextureId::ATTACK, {{640, 128}, 5});
    CORRADE_INTERNAL_ASSERT(m_texture_map.contains(TextureId::ATTACK));
    CORRADE_INTERNAL_ASSERT(m_texture_info_map.contains(TextureId::ATTACK));
    _importTexture(rs, "player_run.png", TextureId::RUN, {{1024, 128}, 8});
    CORRADE_INTERNAL_ASSERT(m_texture_map.contains(TextureId::RUN));
    CORRADE_INTERNAL_ASSERT(m_texture_info_map.contains(TextureId::RUN));
    _importTexture(rs, "player_walk.png", TextureId::WALK, {{1024, 128}, 8});
    CORRADE_INTERNAL_ASSERT(m_texture_map.contains(TextureId::WALK));
    CORRADE_INTERNAL_ASSERT(m_texture_info_map.contains(TextureId::WALK));
  }

  Magnum::GL::Texture2D &get(TextureId textureId) {
    return m_texture_map.at(textureId);
  }

  TextureInfo &getTextureInfo(TextureId textureId) {
    return m_texture_info_map.at(textureId);
  }

private:
  void _importTexture(Corrade::Utility::Resource &rs, const char *fname,
                      TextureId id, TextureInfo info) {

    static Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>
        manager;
    static Corrade::Containers::Pointer<Magnum::Trade::AbstractImporter>
        importer = manager.loadAndInstantiate("StbImageImporter");

    namespace GL = Magnum::GL;
    GL::Texture2D texture;
    if (!importer || !importer->openData(rs.getRaw(fname)))
      Corrade::Utility::Fatal{} << "Could not load " << fname << " texture";

    Corrade::Containers::Optional<Magnum::Trade::ImageData2D> image =
        importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear)
        .setStorage(1, GL::textureFormat(image->format()), image->size())
        .setSubImage(0, {}, *image);

    m_texture_map[id] = std::move(texture);
    m_texture_info_map[id] = info;
  }

  std::unordered_map<TextureId, Magnum::GL::Texture2D> m_texture_map;
  std::unordered_map<TextureId, TextureInfo> m_texture_info_map;
};

#endif // INCLUDE_RESOURCES_TEXTURELOADER_HPP_
