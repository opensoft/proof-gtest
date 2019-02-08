cmake_minimum_required(VERSION 3.12.0)
project(ProofGTest LANGUAGES CXX)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)

add_compile_definitions(GTEST_CREATE_SHARED_LIBRARY)

add_library(proof-gtest SHARED
    gtest/gtest-all.cc
    gtest/proof/test_global.cpp
    gtest/proof/test_fakeserver.cpp

    gtest/gtest-death-test.h
    gtest/gtest-internal-inl.h
    gtest/gtest-message.h
    gtest/gtest-param-test.h
    gtest/gtest-printers.h
    gtest/gtest-spi.h
    gtest/gtest-test-part.h
    gtest/gtest-typed-test.h
    gtest/gtest.h
    gtest/gtest_pred_impl.h
    gtest/gtest_prod.h
    gtest/internal/gtest-death-test-internal.h
    gtest/internal/gtest-filepath.h
    gtest/internal/gtest-internal.h
    gtest/internal/gtest-param-util-generated.h
    gtest/internal/gtest-param-util.h
    gtest/internal/gtest-port.h
    gtest/internal/gtest-string.h
    gtest/internal/gtest-type-util.h
    gtest/internal/gtest-port-arch.h
    gtest/internal/custom/gtest-port.h
    gtest/internal/custom/gtest-printers.h
    gtest/internal/custom/gtest.h
    gtest/proof/test_global.h
    gtest/proof/test_fakeserver.h
)

set_target_properties(proof-gtest PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    AUTOMOC ON
)

target_include_directories(proof-gtest SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)
target_link_libraries(proof-gtest PUBLIC Qt5::Core Qt5::Network Qt5::Test)

install(TARGETS proof-gtest EXPORT proof-gtest-targets
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(EXPORT proof-gtest-targets DESTINATION lib/cmake/3rdparty)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/proof-gtest-config.cmake DESTINATION lib/cmake/3rdparty)
install(DIRECTORY gtest DESTINATION include FILES_MATCHING PATTERN "*.h")
