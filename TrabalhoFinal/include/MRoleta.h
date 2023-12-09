#ifndef MROLETA_H
#define MROLETA_H

#include <Maquina.h>


class MRoleta : public Maquina
{

    public:
        MRoleta(int _id, string _nome, int _x, int _y, int _premio, float _prob, string _tipo, int _aposta);
        virtual ~MRoleta();

    protected:

    private:
};

#endif // MROLETA_H
