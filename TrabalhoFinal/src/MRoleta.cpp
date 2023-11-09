#include "MRoleta.h"

MRoleta::MRoleta(int _nM, int _prob)
    : Maquina(_nM , _prob, "Roleta")
{
    //ctor
    nMaquina = _nM;
    prob = _prob;
}

MRoleta::~MRoleta()
{
    //dtor
}
