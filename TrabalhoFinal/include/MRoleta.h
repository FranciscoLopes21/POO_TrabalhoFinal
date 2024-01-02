#ifndef MROLETA_H
#define MROLETA_H

#include <Maquina.h>
class Casino;


class MRoleta : public Maquina
{

    public:
        MRoleta(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino);
        virtual ~MRoleta();

    protected:

    private:
};

#endif // MROLETA_H
