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
