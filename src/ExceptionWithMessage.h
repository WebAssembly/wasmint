//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_EXCEPTIONWITHDATA_H
#define WASMINT_EXCEPTIONWITHDATA_H


#include <exception>
#include <string>

class ExceptionWithMessage : public std::exception {

    std::string message_;

public:
    ExceptionWithMessage(std::string message) : message_(message) {
    }

    virtual const char* what() const noexcept {
        return message_.c_str();
    }

};


#endif //WASMINT_EXCEPTIONWITHDATA_H
