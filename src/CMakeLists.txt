find_package(Magnum REQUIRED GL Sdl2Application)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(${PROJECT_NAME} 
    EngineCore/Engine.h
    EngineCore/Engine.cpp 
    EngineCore/Renderer.hpp
    EngineCore/SpriteResource.hpp
    Components/Sprite.hpp
    Components/Transform.hpp
    Entities/Entity.hpp
)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::GL
        Magnum::Magnum
        Magnum::Shaders
        Magnum::Primitives
)
