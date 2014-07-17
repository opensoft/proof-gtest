#include "test_global.h"
#include "test_fakeserver.h"

#include <QString>
#include <QByteArray>
#include <QMetaObject>
#include <QThread>

void PrintTo(const QString& str, ::std::ostream* os)
{
    *os << "\"" << str.toStdString() << "\"";
}

void PrintTo(const QByteArray& str, ::std::ostream* os)
{
    *os << "\"" << str.constData() << "\"";
}

FakeServerRunner::FakeServerRunner(int port)
{
    m_serverThread = new QThread();
    m_server = new FakeServer(port);
    m_server->moveToThread(m_serverThread);
    m_serverThread->start();
}

FakeServerRunner::~FakeServerRunner()
{
    stopServer();
    delete m_server;
    delete m_serverThread;
}

void FakeServerRunner::runServer()
{
    QMetaObject::invokeMethod(m_server, "startListen", Qt::BlockingQueuedConnection);
}

void FakeServerRunner::setServerAnswer(const QByteArray &answer)
{
    m_server->setAnswerBody(answer);
}

bool FakeServerRunner::serverIsRunning() const
{
    return m_server->isListening();
}

void FakeServerRunner::setResultCode(int code, const QByteArray &reasonPhrase)
{
    m_server->setResultCode(code, reasonPhrase);
}

QByteArray FakeServerRunner::lastQuery() const
{
    return m_server->lastQuery();
}

void FakeServerRunner::stopServer()
{
    QMetaObject::invokeMethod(m_server, "stopListen", Qt::QueuedConnection);
    m_serverThread->quit();
    m_serverThread->wait(1000);
    m_serverThread->terminate();
}
