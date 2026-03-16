add_executable(csm "src/main.cpp")

# link project_options/warnings
target_link_libraries(csm PRIVATE ogl_project_options ogl_project_warnings)

# Find dependencies:
target_find_dependencies(csm PRIVATE_CONFIG fmt glfw3)
find_package(Stb REQUIRED)
set_target_properties(fmt::fmt PROPERTIES INTERFACE_COMPILE_OPTIONS
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Link dependencies
target_link_system_libraries(csm PRIVATE fmt::fmt glfw)
target_include_directories(csm PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(csm PRIVATE ogl)

# set compile definitions
target_compile_definitions(csm PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
if (${FEATURE_DEVELOPER_MODE})
    target_compile_definitions(csm PRIVATE DEVELOPER_BUILD)
endif ()