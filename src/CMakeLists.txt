find_package(Magnum REQUIRED GL Sdl2Application)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(${PROJECT_NAME} Game.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Corrade::Main)

