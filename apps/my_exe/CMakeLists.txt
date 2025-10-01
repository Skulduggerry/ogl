add_executable(my_exe "src/main.cpp")

# link project_options/warnings
target_link_libraries(my_exe PRIVATE my_project_options my_project_warnings)

# Find dependencies:
target_find_dependencies(my_exe PRIVATE_CONFIG fmt)
set_target_properties(fmt::fmt PROPERTIES INTERFACE_COMPILE_OPTIONS
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)
# Link dependencies
target_link_system_libraries(my_exe PRIVATE fmt::fmt)
target_link_libraries(my_exe PRIVATE my_lib)