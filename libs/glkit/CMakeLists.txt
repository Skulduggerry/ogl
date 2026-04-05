add_library(glkit
        "src/glkit/Camera.cpp"
)

# link project_options/warnings
target_link_libraries(glkit PRIVATE ogl_project_options ogl_project_warnings)
target_compile_options(glkit PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Includes
target_include_interface_directories(glkit include)

# Find dependencies:
# target_find_dependencies(glkit PRIVATE_CONFIG fmt)
target_find_dependencies(glkit PUBLIC_CONFIG glm glad)

# Link dependencies
# target_link_system_libraries(glkit PRIVATE fmt::fmt)
target_link_system_libraries(glkit PUBLIC glm::glm glad::glad)
target_link_libraries(glkit PUBLIC raii-gl)