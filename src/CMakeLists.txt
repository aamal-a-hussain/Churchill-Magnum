find_package(Corrade REQUIRED Main Utility)
find_package(Magnum REQUIRED GL Trade Sdl2Application)

if(NOT TARGET MagnumIntegration::ImGui)
        find_package(MagnumIntegration REQUIRED ImGui)
endif()

if(NOT TARGET MagnumPlugins::StbImageImporter)
        find_package(MagnumPlugins REQUIRED StbImageImporter)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(Game_RESOURCES ${PROJECT_SOURCE_DIR}/resources.conf)


add_executable(${PROJECT_NAME} Game.cpp
        Components/GameComponents.hpp
        Entities/Entity.hpp
        Entities/EntityManager.hpp
        Game.h
        GameSystems.cpp
        Entities/EnemyFab.hpp
        ${Game_RESOURCES}
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
        MagnumPlugins::StbImageImporter)
set(RES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(RES_BINARY_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res")

