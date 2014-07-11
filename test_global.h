#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include "gtest.h"

class QString;
class QByteArray;
class QThread;
class FakeServer;
class QJsonDocument;
//Methods for pretty printing Qt class'es values

void PrintTo(const QString& str, ::std::ostream* os);

class FakeServerRunner {

public:
    FakeServerRunner(int port = 9091);
    ~FakeServerRunner();

    void runServer();
    void setServerAnswer(const QByteArray &answer);
    bool serverIsRunning() const;

private:
    void stopServer();

    QThread *m_serverThread;
    FakeServer *m_server;
};

#endif // TEST_GLOBAL_H
