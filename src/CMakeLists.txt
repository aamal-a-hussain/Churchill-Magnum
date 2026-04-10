find_package(Corrade REQUIRED Main Utility)
find_package(Magnum REQUIRED GL Trade Sdl2Application)
find_package(MagnumPlugins REQUIRED StbImageImporter StbTrueTypeFont)
if(NOT TARGET MagnumIntegration::ImGui)
        find_package(MagnumIntegration REQUIRED ImGui)
endif()



set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(Player_RESOURCES ../res/player_resources.conf)
corrade_add_resource(UI_RESOURCES ../res/ui_resources.conf)

add_executable(${PROJECT_NAME} Engine/GameEngineCore.cpp
        GameComponents.hpp
        Entities/Entity.hpp
        Entities/EntityManager.hpp
        Engine/GameEngine.h
        Engine/GameSystems.cpp
        Entities/EnemyFab.hpp
        ${Player_RESOURCES}
        ${UI_RESOURCES}
        Engine/GameEngineUI.cpp
)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Corrade::Main
        Corrade::Utility
        Magnum::Trade
        Magnum::Application
        Magnum::Primitives
        Magnum::Shaders
        MagnumIntegration::ImGui
        Magnum::GL
        Magnum::Text
)
add_dependencies(${PROJECT_NAME} Magnum::AnyImageImporter
        MagnumPlugins::StbImageImporter MagnumPlugins::StbTrueTypeFont)
