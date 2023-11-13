#include "MRoleta.h"

MRoleta::MRoleta(int _nM, const std::string& _nome, const std::string& _x, const std::string& _y, int _premio, int _prob, const std::string& _tipo)
    : Maquina(_nM, _nome, _x, _y, _premio, _prob, _tipo)
{
    //ctor
}

MRoleta::~MRoleta()
{
    //dtor
}
