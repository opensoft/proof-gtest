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

#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include "test_fakeserver.h"

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QVector>

#include "gtest/gtest.h"

class QObject;
class QThread;
class QSignalSpy;

//Methods for pretty printing Qt class'es values
GTEST_API_ void PrintTo(const QString &str, ::std::ostream *os);
GTEST_API_ void PrintTo(const QByteArray &str, ::std::ostream *os);

GTEST_API_ QStringList findWrongChangedSignalsInQmlWrapper(QObject *obj, const QStringList &excludes = {});
GTEST_API_ QVector<QSignalSpy *> spiesForObject(QObject *obj, const QStringList &excludes = {});
GTEST_API_ QByteArray dataFromFile(const QString &fileName);
GTEST_API_ QByteArray binaryDataFromFile(const QString &fileName);

//Fake Server Runner class
class GTEST_API_ FakeServerRunner // clazy:exclude=rule-of-three
{
public:
    FakeServerRunner(int port = 9091);
    ~FakeServerRunner();

    void runServer();
    void addAnswerHeader(const QString &header, const QString &value);
    void clearAnswerHeaders();
    void setServerAnswer(const QByteArray &answer);
    bool serverIsRunning() const;
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQueryRaw() const;
    QUrl lastQueryUrl() const;
    FakeServer::Method lastQueryMethod() const;
    QByteArray lastQueryBody() const;

private:
    QThread *m_serverThread;
    FakeServer *m_server;
};

#endif // TEST_GLOBAL_H
