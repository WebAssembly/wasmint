

#ifndef WASMINT_EXCEPTIONWITHDATA_H
#define WASMINT_EXCEPTIONWITHDATA_H


#include <exception>
#include <string>
#include <parsing/ByteStream.h>

class ExceptionWithMessage : public std::exception {

    std::string message_;

public:
    ExceptionWithMessage(std::string message) : message_(message) {
    }

    virtual const char* what() const noexcept {
        return message_.c_str();
    }

};

#define ExceptionMessage(Name) class Name : public ExceptionWithMessage { \
    public: Name(std::string message) : ExceptionWithMessage(message) {} \
    };


#endif //WASMINT_EXCEPTIONWITHDATA_H
