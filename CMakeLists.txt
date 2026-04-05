cmake_minimum_required(VERSION 4.0.0)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

# Add project_options from https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
set(PROJECT_OPTIONS_VERSION "v0.41.0")
FetchContent_Declare(
        _project_options
        URL https://github.com/aminya/project_options/archive/refs/tags/${PROJECT_OPTIONS_VERSION}.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# Define the features of the project
include(Features.cmake)

# install vcpkg dependencies: - should be called before defining project()
run_vcpkg(
        VCPKG_URL "https://github.com/microsoft/vcpkg.git"
        VCPKG_REV "0a434205c521ca43d66921271ae2a1d051e718a5"
        VCPKG_DIR "${CMAKE_SOURCE_DIR}/.vcpkg"
        ENABLE_VCPKG_UPDATE)

project(ogl
        VERSION 1.0.0
        LANGUAGES CXX
        DESCRIPTION "The rest of the learnopengl.com projects")

if (FEATURE_DEVELOPER_MODE OR FEATURE_TESTS)
    # enable sanitizers and analyzers if running in developer mode
    set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")

    set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
    set(ENABLE_COVERAGE "ENABLE_COVERAGE")
    set(ENABLE_VS_ANALYSIS "ENABLE_VS_ANALYSIS")

    set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
    set(ENABLE_SANITIZER_UNDEFINED "ENABLE_SANITIZER_UNDEFINED")

    set(WARNINGS_AS_ERRORS "WARNINGS_AS_ERRORS")
endif ()

if (FEATURE_TESTS)
    enable_testing()
    find_package(Catch2 CONFIG REQUIRED)
    add_library(catch2_test_common INTERFACE)
    target_find_dependencies(catch2_test_common INTERFACE_CONFIG Catch2)
    target_link_libraries(catch2_test_common INTERFACE Catch2::Catch2 Catch2::Catch2WithMain)
    target_compile_definitions(catch2_test_common INTERFACE DO_NOT_USE_WMAIN)
    include(Catch)
endif ()

if (FEATURE_DOCS)
    set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif ()

set(CLANG_WARNINGS
        -Wall
        -Wextra # reasonable and standard
        -Wextra-semi # Warn about semicolon after in-class function definition.
        -Wshadow # warn the user if a variable declaration shadows one from a parent context
        -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
        # catch hard to track down memory errors
        -Wold-style-cast # warn for c-style casts
        -Wcast-align # warn for potential performance problem casts
        -Wunused # warn on anything being unused
        -Woverloaded-virtual # warn if you overload (not override) a virtual function
        -Wpedantic # warn if non-standard C++ is used
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wnull-dereference # warn if a null dereference is detected
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output (ie printf)
        -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
)

set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
        -Wuseless-cast # warn if you perform a cast to the same type
        -Wnrvo # warn about not elided copy
)

project_options(
        # defines my_project_options/my_project_warnings
        PREFIX "ogl"
        ENABLE_CACHE
        # ${ENABLE_CPPCHECK}
        # ${ENABLE_CLANG_TIDY}
        ${ENABLE_VS_ANALYSIS}
        # ENABLE_CONAN
        # ENABLE_INTERPROCEDURAL_OPTIMIZATION
        # ENABLE_NATIVE_OPTIMIZATION
        ${ENABLE_DOXYGEN}
        ${ENABLE_COVERAGE}
        ${ENABLE_SANITIZER_ADDRESS}
        ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
        # ${ENABLE_SANITIZER_THREAD}
        # ${ENABLE_SANITIZER_MEMORY}
        # ENABLE_CONTROL_FLOW_PROTECTION
        # ENABLE_STACK_PROTECTION
        # ENABLE_OVERFLOW_PROTECTION
        # ENABLE_ELF_PROTECTION
        # ENABLE_RUNTIME_SYMBOLS_RESOLUTION
        ENABLE_COMPILE_COMMANDS_SYMLINK
        # ENABLE_PCH
        # PCH_HEADERS
        ${WARNINGS_AS_ERRORS}
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_GCC_ANALYZER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # LINKER "lld"
        # CONAN_PROFILE ${profile_path}
        CLANG_WARNINGS "${CLANG_WARNINGS}"
        GCC_WARNINGS "${GCC_WARNINGS}"
)

add_subdirectory(libs/raii-gl)
add_subdirectory(libs/glkit)
add_subdirectory(apps/csm)

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT my_exe)