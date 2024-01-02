#include "MRoleta.h"
#include "Casino.h"

MRoleta::MRoleta(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo, _aposta, _casino)
{
    //ctor
}

MRoleta::~MRoleta()
{
    //dtor
}
