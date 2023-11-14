#ifndef MROLETA_H
#define MROLETA_H

#include <Maquina.h>


class MRoleta : public Maquina
{

    public:
        MRoleta(int _nM, const std::string& _nome, const std::string& _x, const std::string& _y, int _premio, float _prob, const std::string& _tipo);
        virtual ~MRoleta();

    protected:

    private:
};

#endif // MROLETA_H
