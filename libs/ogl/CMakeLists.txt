add_library(ogl
        "src/ogl/Camera.cpp"
        "src/ogl/Framebuffer.cpp"
        "src/ogl/Program.cpp"
        "src/ogl/Renderbuffer.cpp"
        "src/ogl/Shader.cpp"
        "src/ogl/VertexArray.cpp"

        "src/ogl/GlContextCapabilities.cpp"
        "src/ogl/Types.cpp"

        "src/ogl/buffer/BufferStorage.cpp"
        "src/ogl/buffer/UniformBuffer.cpp"
        "src/ogl/buffer/VertexBufferLayout.cpp"

        "src/ogl/texture/Sampler.cpp"
        "src/ogl/texture/TextureObject.cpp"
        "src/ogl/texture/Texture2D.cpp"
        "src/ogl/texture/Texture2DArray.cpp"
)

# link project_options/warnings
target_link_libraries(ogl PRIVATE ogl_project_options ogl_project_warnings)

# Includes
target_include_interface_directories(ogl include)

# Find dependencies:
target_find_dependencies(ogl PRIVATE_CONFIG fmt)
target_find_dependencies(ogl PUBLIC_CONFIG glm glad)
target_compile_options(ogl PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Link dependencies
target_link_system_libraries(ogl PRIVATE fmt::fmt)
target_link_system_libraries(ogl PUBLIC glm::glm glad::glad)

# set compile definitions
if (${FEATURE_DEVELOPER_MODE})
    target_compile_definitions(ogl PRIVATE DEVELOPER_BUILD)
endif ()

if (FEATURE_TESTS)
    #add_subdirectory(test)
endif ()