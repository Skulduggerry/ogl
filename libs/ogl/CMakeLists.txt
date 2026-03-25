add_library(ogl
        "src/ogl/Camera.cpp"
        "src/ogl/Framebuffer.cpp"
        "src/ogl/Program.cpp"
        "src/ogl/Renderbuffer.cpp"
        "src/ogl/Shader.cpp"
        "src/ogl/VertexArray.cpp"

        "src/ogl/texture_old/TextureBase.cpp"
        "src/ogl/texture_old/Texture2D.cpp"
        "src/ogl/texture_old/Texture2DArray.cpp"

        "src/ogl/GlLimits.cpp"
        "src/ogl/Types.cpp"

        "src/ogl/buffer/BufferStorage.cpp"
        "src/ogl/buffer/IndexBuffer.cpp"
        "src/ogl/buffer/UniformBuffer.cpp"
        "src/ogl/buffer/VertexBuffer.cpp"
        "src/ogl/buffer/VertexBufferLayout.cpp"

        "src/ogl/texture/TextureObject.cpp"
)

# link project_options/warnings
target_link_libraries(ogl INTERFACE ogl_project_options ogl_project_warnings)

# Includes
target_include_interface_directories(ogl include)

# Find dependencies:
target_find_dependencies(ogl PRIVATE_CONFIG fmt)
target_find_dependencies(ogl PUBLIC_CONFIG glm glad)
set_target_properties(fmt::fmt PROPERTIES INTERFACE_COMPILE_OPTIONS
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

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