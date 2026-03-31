find_package(Magnum REQUIRED GL Sdl2Application)

if(NOT TARGET MagnumIntegration::ImGui)
        find_package(MagnumIntegration REQUIRED ImGui)
endif()

if(NOT TARGET MagnumPlugins::StbTrueTypeFont)
        find_package(MagnumPlugins REQUIRED StbTrueTypeFont)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(${PROJECT_NAME} Game.cpp
        SpriteLoader.hpp
        Sprite.cpp
        Sprite.h)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::Primitives
        Magnum::Shaders
        MagnumIntegration::ImGui
        Magnum::GL
        Magnum::Text
)
add_dependencies(${PROJECT_NAME} MagnumPlugins::StbTrueTypeFont)
set(RES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(RES_BINARY_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res")


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
        COMMENT "Copying resources to build dir..."
)