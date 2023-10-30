#ifndef CASINO_H
#define CASINO_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

#include "Maquina.h"

class Casino
{
    string nome;
    list<Maquina *> LM;

    public:
        Casino();
        virtual ~Casino();
        void Run();
        void Menu();

    protected:

    private:
};

#endif // CASINO_H
