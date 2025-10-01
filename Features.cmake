# developer mode
option(FEATURE_DEVELOPER_MODE "Enable the developer mode" OFF)

# building the tests
option(FEATURE_TESTS "Enable the tests" OFF)
if(FEATURE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

# building the docs
option(FEATURE_DOCS "Enable the docs" OFF)
