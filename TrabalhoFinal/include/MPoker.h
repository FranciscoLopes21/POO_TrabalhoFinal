#ifndef MPOKER_H
#define MPOKER_H

#include <Maquina.h>


class MPoker : public Maquina
{

    public:
        MPoker(int _id, const std::string& _nome, int _x, int _y, int _premio, int _prob, const std::string& _tipo);
        virtual ~MPoker();

    protected:

    private:
};

#endif // MPOKER_H