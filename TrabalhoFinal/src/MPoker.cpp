#include "MPoker.h"

MPoker::MPoker(int _id, string _nome, int _x, int _y, int _premio, float _prob, string _tipo, int _aposta)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo, _aposta)
{
    //ctor
}

MPoker::~MPoker()
{
    //dtor
}
