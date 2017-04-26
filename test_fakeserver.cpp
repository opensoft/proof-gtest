#include "test_fakeserver.h"

#include <QTcpSocket>

static QSet<QString> METHODS = {
    "GET", "POST", "PATCH", "PUT", "DELETE"
};

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

void FakeServer::addAnswerHeader(const QString &header, const QString &value)
{
    m_headers.append(QStringLiteral("%1: %2\r\n").arg(header, value).toUtf8());
}

void FakeServer::clearAnswerHeaders()
{
    m_headers.clear();
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
        if (METHODS.contains(tokens[0])) {

            forever {
                QByteArray read = socket->read(1024);
                if (read.isEmpty() && !socket->waitForReadyRead(100))
                    break;
                line.append(read);
            }
            m_lastQuery = line;

            socket->write(QString("HTTP/1.0 %1 %2\r\n"
                  "Content-Type: application/json;\r\n").arg(m_returnCode).arg(m_reasonPhrase.constData()).toUtf8());
            for (const QByteArray &header : qAsConst(m_headers))
                socket->write(header);
            socket->write("\r\n");
            socket->write(m_answerBody);
            socket->flush();
            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState)
                socket->deleteLater();
        } else {
            m_lastQuery.append(line);
        }
    }
}
