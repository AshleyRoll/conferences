include(GenerateExportHeader)

add_library(example
        example_api.cpp
        example_api_impl.cpp
        library_global_initializer.cpp
        logger.cpp
        worker.cpp
)

add_library(safe_init::example ALIAS example)


target_link_libraries(example
        PRIVATE
        safe_init::safe_init_options
        safe_init::safe_init_warnings
)

target_link_system_libraries(example
        PRIVATE
        fmt::fmt
)

target_include_directories(example
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>

        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/private>
)

# Ensure that we have default symbol visibility of hidden
set_target_properties(
        example
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(example EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/example/example_export.hpp")
configure_file(include/public/example/version.hpp.in "${CMAKE_BINARY_DIR}/include/example/version.hpp" ESCAPE_QUOTES)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(example PUBLIC EXAMPLE_STATIC_DEFINE)
endif ()