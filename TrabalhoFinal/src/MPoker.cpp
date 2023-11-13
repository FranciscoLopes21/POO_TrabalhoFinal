#include "MPoker.h"

MPoker::MPoker(int _id, const std::string& _nome, int _x, int _y, int _premio, int _prob, const std::string& _tipo)
    : Maquina(_id, _nome, std::to_string(_x), std::to_string(_y), _premio, _prob, _tipo)
{
    //ctor
}

MPoker::~MPoker()
{
    //dtor
}
