#include "XMLReader.h"
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "Casino.h"

using namespace std;

XMLReader::XMLReader()
{
    //ctor
}

XMLReader::~XMLReader()
{
    //dtor
}

bool XMLReader::LoadCasinoData(const string& filename, string& nome, int& maxJogadores, int& probabilidadeUser, int& horaAbertura, int& horaFecho) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        cerr << "Erro ao carregar o arquivo XML: " << result.description() << endl;
        return false;
    }

    pugi::xml_node definicoes = doc.child("DADOS").child("DEFINICOES");

    nome = definicoes.child("NOME").text().get();
    maxJogadores = definicoes.child("MAX_JOG").text().as_int();
    probabilidadeUser = definicoes.child("PROB_USER").text().as_int();
    horaAbertura = definicoes.child("HORA_INICIO").text().as_int();
    horaFecho = definicoes.child("HORA_FIM").text().as_int();

    return true;
}

