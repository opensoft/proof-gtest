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

#ifndef TEST_FAKESERVER_H
#define TEST_FAKESERVER_H

#include <QUrl>
#include <QtNetwork/QTcpServer>

class FakeServer : public QTcpServer
{
    Q_OBJECT
    Q_ENUMS(Method)
public:
    enum class Method
    {
        Get,
        Post,
        Patch,
        Put,
        Delete,
        Custom
    };

    FakeServer(int port = 9091);

    Q_INVOKABLE void startListen();
    Q_INVOKABLE void stopListen();
    void addAnswerHeader(const QString &header, const QString &value);
    void clearAnswerHeaders();
    void setAnswerBody(const QByteArray &rawAnswer);
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQueryRaw() const;
    QUrl lastQueryUrl() const;
    Method lastQueryMethod() const;
    QByteArray lastQueryBody() const;

private:
    void createNewConnection();
    void removeConnection();
    void sendData();

    int m_port;
    QVector<QByteArray> m_headers;
    QByteArray m_answerBody;
    QByteArray m_lastQueryRaw;
    QUrl m_lastQueryUrl;
    Method m_lastQueryMethod;
    QByteArray m_lastQueryBody;
    int m_returnCode;
    QByteArray m_reasonPhrase;
};

#endif // TEST_FAKESERVER_H
