#include "MPoker.h"

MPoker::MPoker(int _nM, int _prob)
    : Maquina(_nM , _prob, "Poker")
{
    //ctor
    nMaquina = _nM;
    prob = _prob;
}

MPoker::~MPoker()
{
    //dtor
}
