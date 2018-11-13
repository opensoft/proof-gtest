include(CMakeFindDependencyMacro)

find_dependency(Qt5Core CONFIG REQUIRED)
find_dependency(Qt5Network CONFIG REQUIRED)
find_dependency(Qt5Test CONFIG REQUIRED)

if(NOT TARGET proof-gtest)
    include("${CMAKE_CURRENT_LIST_DIR}/proof-gtest-targets.cmake")
endif()
