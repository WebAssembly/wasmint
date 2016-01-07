#ifndef WASMINT_DATARESCUER_H
#define WASMINT_DATARESCUER_H

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include <iostream>

namespace wasmint {

    class DataRescuer;

    DataRescuer* rescuer = nullptr;

    void sig_handler(int signo);

    class DataRescuer {

    public:
        unsigned long seed_;

        DataRescuer(unsigned long seed) : seed_(seed) {
            if (rescuer != nullptr) {
                std::cerr << "Multiple DataRescuer exist at the same time! Aborting...." << std::endl;
                abort();
            }
            rescuer = this;
        }
        virtual ~DataRescuer() {
            rescuer = nullptr;
        }

        static void attachHandler() {
            if (signal(SIGINT, sig_handler) == SIG_ERR)
                std::cerr << "can't catch SIGINT\n";
            if (signal(SIGSEGV, sig_handler) == SIG_ERR)
                std::cerr << "can't catch SIGSEGV\n";
            if (signal(SIGILL, sig_handler) == SIG_ERR)
                std::cerr << "can't catch SIGILL\n";
        }

    };


    void sig_handler(int signo)
    {
        std::cerr << "received ";
        if (signo == SIGINT)
            std::cerr << "SIGINT";
        if (signo == SIGSEGV)
            std::cerr << "SIGSEGV";
        std::cerr << " with seed:" << rescuer->seed_ << std::endl;
        exit(1);
    }


}

#endif //WASMINT_DATARESCUER_H
