#ifndef XMLREADER_H
#define XMLREADER_H
#include <iostream>
#include <string>
#include "pugixml.hpp"

using namespace std;

class XMLReader
{
    public:
        XMLReader();
        virtual ~XMLReader();
        static bool LoadCasinoData(const string& filename, string& nome, int& maxJogadores, int& probabilidadeUser, int& horaAbertura, int& horaFecho);

    protected:

    private:
};

#endif // XMLREADER_H
