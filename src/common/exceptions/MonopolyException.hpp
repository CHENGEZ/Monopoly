#ifndef MONOPOLY_EXCEPTION_H
#define MONOPOLY_EXCEPTION_H

#include <string>

class MonopolyException : public std::exception {
private:
    std::string _msg;
public:
    explicit MonopolyException(const std::string& message) : _msg(message) { };

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

#endif //MONOPOLY_EXCEPTION_H
