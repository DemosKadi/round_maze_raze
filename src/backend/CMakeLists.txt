find_package(gcem CONFIG REQUIRED)

set(LIB_NAME backend)

add_library(${LIB_NAME} INTERFACE)

target_sources(${LIB_NAME} INTERFACE
        position.h
        controller_model.h
        config_model.h
        )

    #target_include_directories(${LIB_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)

target_link_libraries(${LIB_NAME} INTERFACE
        project_options
        project_warnings
        gcem::gcem
        )
