#include "test_global.h"

#include <QString>

void PrintTo(const QString& str, ::std::ostream* os)
{
    *os << "\"" << str.toStdString() << "\"";
}

