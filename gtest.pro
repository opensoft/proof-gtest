TEMPLATE = lib
TARGET = gtest
CONFIG += qt
QT += core network

include(../../proof.pri)

DESTDIR = $$BUILDPATH/tests

DEPENDPATH += . internal
INCLUDEPATH += .. .

QMAKE_CXXFLAGS += -Wno-missing-field-initializers

# Input
HEADERS += gtest-death-test.h \
           gtest-internal-inl.h \
           gtest-message.h \
           gtest-param-test.h \
           gtest-printers.h \
           gtest-spi.h \
           gtest-test-part.h \
           gtest-typed-test.h \
           gtest.h \
           gtest_pred_impl.h \
           gtest_prod.h \
           internal/gtest-death-test-internal.h \
           internal/gtest-filepath.h \
           internal/gtest-internal.h \
           internal/gtest-linked_ptr.h \
           internal/gtest-param-util-generated.h \
           internal/gtest-param-util.h \
           internal/gtest-port.h \
           internal/gtest-string.h \
           internal/gtest-tuple.h \
           internal/gtest-type-util.h \
           test_global.h \
    test_fakeserver.h
SOURCES += gtest-all.cc \
           gtest-death-test.cc \
           gtest-filepath.cc \
           gtest-port.cc \
           gtest-printers.cc \
           gtest-test-part.cc \
           gtest-typed-test.cc \
           gtest.cc \
           test_global.cpp \
    test_fakeserver.cpp
