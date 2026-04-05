add_library(GLKit
        "src/GLKit/Camera.cpp"
)

# link project_options/warnings
target_link_libraries(GLKit PRIVATE ogl_project_options ogl_project_warnings)
target_compile_options(GLKit PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Includes
target_include_interface_directories(GLKit include)

# Find dependencies:
# target_find_dependencies(GLKit PRIVATE_CONFIG fmt)
target_find_dependencies(GLKit PUBLIC_CONFIG glm glad)

# Link dependencies
# target_link_system_libraries(GLKit PRIVATE fmt::fmt)
target_link_system_libraries(GLKit PUBLIC glm::glm glad::glad)
target_link_libraries(GLKit PUBLIC raii-gl)