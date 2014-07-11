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
    void setRawAnswer(const QByteArray &rawAnswer);

private slots:
    void createNewConnection();
    void removeConnection();
    void sendData();

private:
    int m_port;
    QByteArray m_rawAnswer;
};

#endif // TEST_FAKESERVER_H
