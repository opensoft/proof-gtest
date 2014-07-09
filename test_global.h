#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include "gtest.h"

class QString;
//Methods for pretty printing Qt class'es values

void PrintTo(const QString& str, ::std::ostream* os);

#endif // TEST_GLOBAL_H
