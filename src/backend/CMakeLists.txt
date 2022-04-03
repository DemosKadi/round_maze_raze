find_package(gcem CONFIG REQUIRED)

set(LIB_NAME backend)

add_library(${LIB_NAME} SHARED)

target_sources(${LIB_NAME}
        PRIVATE
        position.cpp
        tilting_model.cpp
        PUBLIC
        position.h
        tilting_model.h
        )

target_link_libraries(${LIB_NAME}
        PUBLIC
        project_options
        project_warnings
        gcem::gcem
        )