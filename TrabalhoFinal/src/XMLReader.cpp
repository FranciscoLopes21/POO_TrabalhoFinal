#include "XMLReader.h"
#include <iostream>
#include <string>
#include <sstream>  // Adicione esta linha para incluir a biblioteca <sstream>
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

bool XMLReader::LoadCasinoData(const string& filename, int& maxJogadores, int& probabilidadeUser,
                               int& horaAbertura,int& minutoAbertura,int& segundoAbertura,
                               int& horaFecho, int& minutoFecho, int& segundoFecho) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        cout << "Erro ao carregar o arquivo XML: " << result.description() << endl;
        return false;
    }

    pugi::xml_node definicoes = doc.child("DADOS").child("DEFINICOES");

    //nome = definicoes.child("NOME").text().get();
    maxJogadores = definicoes.child("MAX_JOG").text().as_int();
    probabilidadeUser = definicoes.child("PROB_USER").text().as_int();
    //horaAbertura = definicoes.child("HORA_INICIO").text().as_int();
    // Extrair horas, minutos e segundos da string no formato "hh:mm:ss"
    string horaAberturaStr = definicoes.child("HORA_INICIO").text().as_string();
    istringstream horaStream(horaAberturaStr);
    char delimiterA;
    horaStream >> horaAbertura >> delimiterA >> minutoAbertura >> delimiterA >> segundoAbertura;

    //horaFecho = definicoes.child("HORA_FIM").text().as_int();
    string horaFechoStr = definicoes.child("HORA_FIM").text().as_string();
    istringstream horaFStream(horaFechoStr);
    char delimiterF;
    horaFStream >> horaFecho >> delimiterF >> minutoFecho >> delimiterF >> segundoFecho;

    return true;
}

