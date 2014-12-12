#include "test_fakeserver.h"

#include <QTcpSocket>

FakeServer::FakeServer(int port)
    : m_port(port),
      m_returnCode(200),
      m_reasonPhrase("OK")
{
    connect(this, &FakeServer::newConnection, this, &FakeServer::createNewConnection);
}

void FakeServer::startListen()
{
    if (!isListening())
        listen(QHostAddress::Any, m_port);
}

void FakeServer::stopListen()
{
    close();
}

void FakeServer::setAnswerBody(const QByteArray &rawAnswer)
{
    m_answerBody = rawAnswer;
}

void FakeServer::setResultCode(int code, const QByteArray &reasonPhrase)
{
    m_returnCode = code;
    m_reasonPhrase = reasonPhrase;
}

QByteArray FakeServer::lastQuery() const
{
    return m_lastQuery;
}

void FakeServer::createNewConnection()
{
    if (hasPendingConnections()) {
        QTcpSocket *socket = nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &FakeServer::sendData);
        connect(socket, &QTcpSocket::disconnected, this, &FakeServer::removeConnection);
    }
}

void FakeServer::removeConnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    socket->deleteLater();
}

void FakeServer::sendData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket && socket->canReadLine()) {
        QByteArray line = socket->readLine();
        QStringList tokens = QString(line).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "PATCH") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << QString("HTTP/1.0 %1 %2\r\n"
                  "Content-Type: text/json; charset=\"utf-8\"\r\n"
                  "\r\n").arg(m_returnCode).arg(m_reasonPhrase.constData())
               << m_answerBody;

            forever {
                QByteArray read = socket->read(1024);
                if (read.isEmpty() && !socket->waitForReadyRead(100))
                    break;
                line.append(read);
            }

            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
            m_lastQuery = line;
        }
    }
}
