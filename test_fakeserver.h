#ifndef TEST_FAKESERVER_H
#define TEST_FAKESERVER_H

#include <QTcpServer>

class FakeServer : public QTcpServer
{
    Q_OBJECT
public:
    FakeServer(int port = 9091);

    Q_INVOKABLE void startListen();
    Q_INVOKABLE void stopListen();
    void addAnswerHeader(const QString &header, const QString &value);
    void clearAnswerHeaders();
    void setAnswerBody(const QByteArray &rawAnswer);
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQuery() const;

private:
    void createNewConnection();
    void removeConnection();
    void sendData();

    int m_port;
    QVector<QByteArray> m_headers;
    QByteArray m_answerBody;
    QByteArray m_lastQuery;
    int m_returnCode;
    QByteArray m_reasonPhrase;
};

#endif // TEST_FAKESERVER_H
