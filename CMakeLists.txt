cmake_minimum_required(VERSION 4.0.0)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_CXX_STANDARD 23)
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
        VCPKG_REV "0d9d4684352ba8de70bdf251c6fc9a3c464fa12b"
        VCPKG_DIR "${CMAKE_SOURCE_DIR}/.vcpkg"
        ENABLE_VCPKG_UPDATE)

project(ogl
        VERSION 1.0.0
        LANGUAGES CXX
        DESCRIPTION "The rest of the learnopengl.com projects")

if (FEATURE_DEVELOPER_MODE OR FEATURE_TESTS)
    # enable sanitizers and analyzers if running in developer mode
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
    endif ()

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

if(FEATURE_DOCS)
    set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()

project_options(
        # defines my_project_options/my_project_warnings
        PREFIX "ogl"
        ENABLE_CACHE
        ${ENABLE_CPPCHECK}
        ${ENABLE_CLANG_TIDY}
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
)

add_subdirectory(libs/ogl)
add_subdirectory(apps/csm)

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT my_exe)