#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include "gtest.h"
#include <QList>
#include <QSignalSpy>
#include <QStringList>
#include <QString>
#include <QByteArray>

class QString;
class QByteArray;
class QThread;
class FakeServer;
class QJsonDocument;

//Methods for pretty printing Qt class'es values
void PrintTo(const QString& str, ::std::ostream* os);
void PrintTo(const QByteArray& str, ::std::ostream* os);

QList<QSignalSpy *> spiesForObject(QObject *obj, const QStringList &excludes = {});
QByteArray dataFromFile(const QString &fileName);

//Fake Server Runner class
class FakeServerRunner {

public:
    FakeServerRunner(int port = 9091);
    ~FakeServerRunner();

    void runServer();
    void setServerAnswer(const QByteArray &answer);
    bool serverIsRunning() const;
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQuery() const;

private:
    QThread *m_serverThread;
    FakeServer *m_server;
};

#endif // TEST_GLOBAL_H
