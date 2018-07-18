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
