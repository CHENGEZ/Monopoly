//
// Created by Ricky on 2023/5/13.
//

#ifndef MONOPOLY_CONNECTIONTHREAD_H
#define MONOPOLY_CONNECTIONTHREAD_H

#include <QThread>

class ConnectionThread : public QThread {
    Q_OBJECT
public:
    ConnectionThread(const std::string& host, const uint16_t port) : _host(host), _port(port){}

signals:
    void connected(bool success);

protected:
    void run() override;


private:
    const std::string _host;
    const uint16_t _port;
};


#endif //MONOPOLY_CONNECTIONTHREAD_H
