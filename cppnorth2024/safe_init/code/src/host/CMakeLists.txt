
add_executable(host
        main.cpp
)

target_link_libraries(host
        PRIVATE
        safe_init::safe_init_options
        safe_init::safe_init_warnings

        safe_init::example
)

target_link_system_libraries(host
        PRIVATE
        fmt::fmt
)
