TEMPLATE = lib
TARGET = proof-gtest
CONFIG += qt c++14
QT += core network testlib
QT -= gui

DEPENDPATH += . gtest gtest/internal
INCLUDEPATH += . gtest

msvc {
    DEFINES += GTEST_CREATE_SHARED_LIBRARY
} else {
    QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-field-initializers
}

HEADERS += \
    gtest/gtest-death-test.h \
    gtest/gtest-internal-inl.h \
    gtest/gtest-message.h \
    gtest/gtest-param-test.h \
    gtest/gtest-printers.h \
    gtest/gtest-spi.h \
    gtest/gtest-test-part.h \
    gtest/gtest-typed-test.h \
    gtest/gtest.h \
    gtest/gtest_pred_impl.h \
    gtest/gtest_prod.h \
    gtest/internal/gtest-death-test-internal.h \
    gtest/internal/gtest-filepath.h \
    gtest/internal/gtest-internal.h \
    gtest/internal/gtest-param-util-generated.h \
    gtest/internal/gtest-param-util.h \
    gtest/internal/gtest-port.h \
    gtest/internal/gtest-string.h \
    gtest/internal/gtest-type-util.h \
    gtest/internal/gtest-port-arch.h \
    gtest/internal/custom/gtest-port.h \
    gtest/internal/custom/gtest-printers.h \
    gtest/internal/custom/gtest.h \
    gtest/proof/test_global.h \
    gtest/proof/test_fakeserver.h

SOURCES += gtest/gtest-all.cc \
    gtest/gtest-death-test.cc \
    gtest/gtest-filepath.cc \
    gtest/gtest-port.cc \
    gtest/gtest-printers.cc \
    gtest/gtest-test-part.cc \
    gtest/gtest-typed-test.cc \
    gtest/gtest.cc \
    gtest/proof/test_global.cpp \
    gtest/proof/test_fakeserver.cpp

PROOF_GTEST_PREFIX = $$(PROOF_PATH)
isEmpty(PROOF_GTEST_PREFIX) {
    # Standalone
    !build_pass:log(Building proof-gtest as standalone library $$escape_expand(\\n))
    target.path = $$PREFIX/lib/
    headers.path = $$PREFIX/include/gtest
    headers.files = gtest/*.h *.h
    internal_headers.path = $$PREFIX/include/gtest/internal
    internal_headers.files = gtest/internal/*.h
    INSTALLS += target headers internal_headers
} else {
    # Part of Proof
    !build_pass:log(Building proof-gtest as part of Proof framework $$escape_expand(\\n))
    DESTDIR = $$PROOF_GTEST_PREFIX/lib
}

