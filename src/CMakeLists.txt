find_package(Magnum REQUIRED GL Sdl2Application)
find_package(MagnumPlugins REQUIRED StbImageImporter)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(Player_RESOURCES ../res/player_data.conf)

add_executable(${PROJECT_NAME} 
    EngineCore/Main.cpp 
    Input/InputSystem.cpp 
    Resources/SpriteResource.cpp
    Renderer.cpp
    ${Player_RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::GL
        Magnum::Magnum
        Magnum::Shaders
        Magnum::Primitives
        # MagnumPlugins::StbImageImporter
)
add_dependencies(${PROJECT_NAME} MagnumPlugins::StbImageImporter)
