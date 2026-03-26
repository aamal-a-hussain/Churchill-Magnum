find_package(Magnum REQUIRED GL Sdl2Application)

if(NOT TARGET MagnumIntegration::ImGui)
        find_package(MagnumIntegration REQUIRED ImGui)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(${PROJECT_NAME} Game.cpp)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::Primitives
        Magnum::Shaders
        MagnumIntegration::ImGui
        Magnum::GL
)

