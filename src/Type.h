//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPE_H
#define WASMINT_TYPE_H



class Type {

    static Type Int32_;
    static Type Void_;

    Type() {

    }

public:
    static const Type& Int32() {
        return Int32_;
    }

    static const Type& Void() {
        return Void_;
    }

    bool operator==(Type& other) const {
        return this == &other;
    }

};


#endif //WASMINT_TYPE_H
