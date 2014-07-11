#include "test_global.h"
#include "test_fakeserver.h"

#include <QString>
#include <QMetaObject>
#include <QThread>

void PrintTo(const QString& str, ::std::ostream* os)
{
    *os << "\"" << str.toStdString() << "\"";
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
    QMetaObject::invokeMethod(m_server, "startListen", Qt::QueuedConnection);
}

void FakeServerRunner::setServerAnswer(const QByteArray &answer)
{
    m_server->setRawAnswer(answer);
}

bool FakeServerRunner::serverIsRunning() const
{
    return m_server->isListening();
}

void FakeServerRunner::stopServer()
{
    QMetaObject::invokeMethod(m_server, "stopListen", Qt::QueuedConnection);
    m_serverThread->quit();
    m_serverThread->wait(1000);
    m_serverThread->terminate();
}
