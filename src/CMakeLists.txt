find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(docopt CONFIG)
find_package(gcem CONFIG)

# Generic test that uses conan libs
add_executable(intro)

target_sources(intro
        PRIVATE main.cpp
                position.h
                position.cpp
                tilting_model.h
                tilting_model.cpp)

target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
          docopt::docopt
          fmt::fmt
          spdlog::spdlog
          gcem::gcem)

target_link_system_libraries(
  intro
  PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
