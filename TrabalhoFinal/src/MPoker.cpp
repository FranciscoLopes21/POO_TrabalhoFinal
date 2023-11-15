#include "MPoker.h"

MPoker::MPoker(int _id, const std::string& _nome, int _x, int _y, int _premio, float _prob, const std::string& _tipo)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo)
{
    //ctor
}

MPoker::~MPoker()
{
    //dtor
}
