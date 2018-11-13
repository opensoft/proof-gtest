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

#include "test_global.h"

#include "test_fakeserver.h"

#include <QFile>
#include <QMetaObject>
#include <QSignalSpy>
#include <QThread>

void PrintTo(const QString &str, ::std::ostream *os)
{
    *os << "\"" << str.toStdString() << "\"";
}

void PrintTo(const QByteArray &str, ::std::ostream *os)
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
    QMetaObject::invokeMethod(m_server, "stopListen", Qt::BlockingQueuedConnection);
    m_server->deleteLater();
    m_serverThread->quit();
    m_serverThread->wait(1000);
    if (m_serverThread->isRunning())
        m_serverThread->terminate();
    delete m_serverThread;
}

void FakeServerRunner::runServer()
{
    QMetaObject::invokeMethod(m_server, "startListen", Qt::BlockingQueuedConnection);
}

void FakeServerRunner::addAnswerHeader(const QString &header, const QString &value)
{
    m_server->addAnswerHeader(header, value);
}

void FakeServerRunner::clearAnswerHeaders()
{
    m_server->clearAnswerHeaders();
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

QByteArray FakeServerRunner::lastQueryRaw() const
{
    return m_server->lastQueryRaw();
}

QUrl FakeServerRunner::lastQueryUrl() const
{
    return m_server->lastQueryUrl();
}

FakeServer::Method FakeServerRunner::lastQueryMethod() const
{
    return m_server->lastQueryMethod();
}

QByteArray FakeServerRunner::lastQueryBody() const
{
    return m_server->lastQueryBody();
}

QVector<QSignalSpy *> spiesForObject(QObject *obj, const QStringList &excludes)
{
    QVector<QSignalSpy *> spies;
    for (int i = obj->metaObject()->methodOffset(); i < obj->metaObject()->methodCount(); ++i) {
        if (obj->metaObject()->method(i).methodType() == QMetaMethod::Signal) {
            QByteArray sign = obj->metaObject()->method(i).methodSignature();
            if (excludes.contains(sign))
                continue;
            //HACK: Because QSignalSpy can't signals without SIGNAL() macros, but this hack cheating it
            sign.prepend("2");
            spies << new QSignalSpy(obj, qFlagLocation(sign.constData()));
        }
    }
    return spies;
}

QStringList findWrongChangedSignalsInQmlWrapper(QObject *obj, const QStringList &excludes)
{
    const QMetaObject *metaObject = obj->metaObject();
    QStringList invalidProperties;
    for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        QMetaProperty property = metaObject->property(i);
        QString name = QString::fromLatin1(property.name());

        if (excludes.contains(name) || property.isConstant())
            continue;

        QString signal = QString::fromLatin1(property.notifySignal().name());
        if (QStringLiteral("%1Changed").arg(name) != signal)
            invalidProperties << QStringLiteral("%1=>%2").arg(name, signal);
    }

    return invalidProperties;
}

QByteArray dataFromFile(const QString &fileName)
{
    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return QByteArray();
    QByteArray data = jsonFile.readAll();
    jsonFile.close();
    return data;
}

QByteArray binaryDataFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return QByteArray();
    QByteArray data = file.readAll();
    file.close();
    return data;
}
