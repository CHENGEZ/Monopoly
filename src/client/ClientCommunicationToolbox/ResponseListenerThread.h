#ifndef RESPONSELISTENERTHREAD_H
#define RESPONSELISTENERTHREAD_H

#include <QThread>
#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"

class ResponseListenerThread : public QThread
{
    Q_OBJECT




public:
    explicit ResponseListenerThread(QObject* parent = nullptr, sockpp::tcp_connector* connection= nullptr);
    ~ResponseListenerThread();
    void run() override;
signals:
    void recievedVaildFeedback(const std::string& feedback);

private:
    void outputError(std::string title, std::string message);
    sockpp::tcp_connector* _connection;


};

#endif // RESPONSELISTENERTHREAD_H
