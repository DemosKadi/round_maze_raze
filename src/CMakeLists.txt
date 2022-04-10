find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(docopt CONFIG)

#find_library(atomic-lib atomic)
# gcem has to be found and linked here, else it does not work
# find_package(gcem CONFIG)

# Generic test that uses conan libs
add_executable(intro main.cpp)

target_link_libraries(
        intro
        PRIVATE project_options
        project_warnings
        docopt::docopt
        fmt::fmt
        spdlog::spdlog
        atomic
        INTERFACE backend)

target_link_system_libraries(
        intro
        PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")


#add_library(intro_lib STATIC)
#target_sources(intro_lib
#PUBLIC position.h
#tilting_model.h
#PRIVATE position.cpp
#tilting_model.cpp)

#target_link_libraries(intro_lib
#PUBLIC gcem::gcem
#PRIVATE project_options
#project_warnings)
