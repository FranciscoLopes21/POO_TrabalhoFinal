#include "MSlot.h"

MSlot::MSlot(int _nM, int _prob)
    : Maquina(_nM , _prob, "Slots de recursos")
{
    //ctor
    nMaquina = _nM;
    prob = _prob;
}

MSlot::~MSlot()
{
    //dtor
}
