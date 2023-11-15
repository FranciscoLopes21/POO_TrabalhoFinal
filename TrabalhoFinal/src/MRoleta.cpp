#include "MRoleta.h"

MRoleta::MRoleta(int _nM, const std::string& _nome, int _x, int _y, int _premio, float _prob, const std::string& _tipo)
    : Maquina(_nM, _nome, _x, _y, _premio, _prob, _tipo)
{
    //ctor
}

MRoleta::~MRoleta()
{
    //dtor
}
