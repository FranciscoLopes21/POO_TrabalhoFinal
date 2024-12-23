#ifndef XMLREADER_H
#define XMLREADER_H
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "Casino.h"

using namespace std;

class XMLReader
{
    public:
        XMLReader();
        virtual ~XMLReader();
        static bool LoadCasinoData(const string& nomeFicheiro, int& maxJogadores, int& horaAbertura,
                                   int& minutosAbertura, int& segundosAbertura,
                                   int& horaFecho, int& minutosFecho, int& segundosFecho);
    protected:

    private:
};

#endif // XMLREADER_H
