#include "MBlackJack.h"

MBlackJack::MBlackJack(int _nM, int _prob)
    : Maquina(_nM , _prob, "BlackJack")
{
    //ctor
    nMaquina = _nM;
    prob = _prob;
}

MBlackJack::~MBlackJack()
{
    //dtor
}
