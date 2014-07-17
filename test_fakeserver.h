#ifndef TEST_FAKESERVER_H
#define TEST_FAKESERVER_H

#include <QTcpServer>

class FakeServer: public QTcpServer
{
    Q_OBJECT
public:
    FakeServer(int port = 9091);

public slots:
    void startListen();
    void stopListen();
    void setAnswerBody(const QByteArray &rawAnswer);
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQuery() const;

private slots:
    void createNewConnection();
    void removeConnection();
    void sendData();

private:
    int m_port;
    QByteArray m_answerBody;
    QByteArray m_lastQuery;
    int m_returnCode;
    QByteArray m_reasonPhrase;
};

#endif // TEST_FAKESERVER_H
