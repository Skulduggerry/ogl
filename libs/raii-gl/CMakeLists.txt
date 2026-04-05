add_library(raii-gl
        "src/raii-gl/buffer/BufferStorage.cpp"
        "src/raii-gl/buffer/UniformBuffer.cpp"

        "src/raii-gl/render/Framebuffer.cpp"
        "src/raii-gl/render/Program.cpp"
        "src/raii-gl/render/Renderbuffer.cpp"
        "src/raii-gl/render/Shader.cpp"
        "src/raii-gl/render/VertexArray.cpp"
        "src/raii-gl/render/VertexBufferLayout.cpp"

        "src/raii-gl/texture/Sampler.cpp"
        "src/raii-gl/texture/Texture2D.cpp"
        "src/raii-gl/texture/Texture2DArray.cpp"
        "src/raii-gl/texture/TextureObject.cpp"

        "src/raii-gl/GlContextCapabilities.cpp"
        "src/raii-gl/Types.cpp"
)

# link project_options/warnings
target_link_libraries(raii-gl PRIVATE ogl_project_options ogl_project_warnings)
target_compile_options(raii-gl PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Includes
target_include_interface_directories(raii-gl include)

# Find dependencies:
target_find_dependencies(raii-gl PRIVATE_CONFIG fmt)
target_find_dependencies(raii-gl PUBLIC_CONFIG glm glad)

# Link dependencies
target_link_system_libraries(raii-gl PRIVATE fmt::fmt)
target_link_system_libraries(raii-gl PUBLIC glm::glm glad::glad)

# set compile definitions
if (${FEATURE_DEVELOPER_MODE})
    target_compile_definitions(raii-gl PRIVATE DEVELOPER_BUILD)
endif ()