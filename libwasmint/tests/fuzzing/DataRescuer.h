#ifndef WASMINT_DATARESCUER_H
#define WASMINT_DATARESCUER_H

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include <iostream>

namespace wasmint {

    class DataRescuer;

    extern DataRescuer* rescuer;

    void sig_handler(int signo);

    class DataRescuer {

    public:
        long long seed_;

        DataRescuer() {
            if (rescuer != nullptr) {
                std::cerr << "Multiple DataRescuer exist at the same time! Aborting...." << std::endl;
                abort();
            }
            rescuer = this;
        }
        virtual ~DataRescuer() {
            rescuer = nullptr;
        }

        void setSeed(long long seed) {
            seed_ = seed;
        }

        static void attachHandler() {
            if (signal(SIGSEGV, sig_handler) == SIG_ERR)
                std::cerr << "can't catch SIGSEGV\n";
            if (signal(SIGILL, sig_handler) == SIG_ERR)
                std::cerr << "can't catch SIGILL\n";
            // TODO more signals
        }

    };

    inline void sig_handler(int signo)
    {
        std::cerr << "received ";
        if (signo == SIGSEGV)
            std::cerr << "SIGSEGV";
        else if (signo == SIGILL)
            std::cerr << "SIGILL";
        else
            std::cerr << "Unknown signal with number " << signo;
        std::cerr << " with seed:" << rescuer->seed_ << std::endl;
        exit(1);
    }

}

#endif //WASMINT_DATARESCUER_H
