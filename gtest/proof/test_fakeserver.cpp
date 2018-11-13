// Copyright 2018, OpenSoft Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of OpenSoft Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// clazy:skip

#include "test_fakeserver.h"

#include <QTcpSocket>

static const QHash<QString, FakeServer::Method> METHODS = {{"GET", FakeServer::Method::Get},
                                                           {"POST", FakeServer::Method::Post},
                                                           {"PATCH", FakeServer::Method::Patch},
                                                           {"PUT", FakeServer::Method::Put},
                                                           {"DELETE", FakeServer::Method::Delete}};

FakeServer::Method methodFromString(QString methodString)
{
    return METHODS.value(methodString, FakeServer::Method::Custom);
}

FakeServer::FakeServer(int port) : m_port(port), m_returnCode(200), m_reasonPhrase("OK")
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

QByteArray FakeServer::lastQueryRaw() const
{
    return m_lastQueryRaw;
}

QUrl FakeServer::lastQueryUrl() const
{
    return m_lastQueryUrl;
}

FakeServer::Method FakeServer::lastQueryMethod() const
{
    return m_lastQueryMethod;
}

QByteArray FakeServer::lastQueryBody() const
{
    return m_lastQueryBody;
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
        m_lastQueryMethod = methodFromString(tokens[0]);
        if (tokens.count() > 1)
            m_lastQueryUrl = tokens[1];
        forever {
            QByteArray read = socket->read(1024);
            if (read.isEmpty() && !socket->waitForReadyRead(100))
                break;
            line.append(read);
        }
        m_lastQueryRaw = line;
        m_lastQueryBody = QString::fromLatin1(m_lastQueryRaw).split("\r\n\r\n").last().toLatin1();

        if (m_lastQueryMethod != Method::Custom) {
            socket->write(QStringLiteral("HTTP/1.0 %1 %2\r\nContent-Type: application/json;\r\n")
                              .arg(m_returnCode)
                              .arg(m_reasonPhrase.constData())
                              .toUtf8());
            for (const QByteArray &header : qAsConst(m_headers))
                socket->write(header);
            socket->write("\r\n");
            socket->write(m_answerBody);
            socket->flush();
            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState)
                socket->deleteLater();
        }
    }
}
