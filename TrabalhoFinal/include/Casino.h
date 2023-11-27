#ifndef CASINO_H
#define CASINO_H
#include <iostream>
using namespace std;
#include <string>
#include <list>
#include "pugixml.hpp"

#include "Maquina.h"
#include "User.h"
#include <random>  // Adicione esta linha para incluir a biblioteca <random>

class Casino
{
    string nome;
    int maxJogadores;
    int probabilidadeUser;
    int horaAbertura;
    int horaFecho;
    list<Maquina *> LM;
    list<Maquina *> LM_Avariadas;
    list<User *> LU;
    list<int> Lt;


    public:
        Casino(string _nome);
        void CarregarDados(int _maxJogadores, int _probabilidadeUser,int _horaAbertura, int _horaFecho);
        void dadosCasino();
        bool LoadMachinesFromXML(const string& xmlFile);
        void ListMachines() const;
        virtual ~Casino();
        string getNome();
        void Run();
        bool Add(Maquina *M);
        void maquinaAvariada();
        void maquinaGanhos();
        void reparar();
        void registarMaquina();
        void Desligar(int id_maq);
        estadoMaquina Get_Estado(int id_maq);
        string estadoString(estadoMaquina estadoma);
        void Listar(float X, std::ostream &f = std::cout);
        void ListarMaquinasProbabilidade();

        void avariar(int nMaq);
        list<string> * Ranking_Dos_Fracos();
        void showRankingAvarias();

        void listarTipoMaquina();
        list<Maquina *> Listar_Tipo(string Tipo, std::ostream &f = std::cout);

        //Menus
        void Menu();
        void gestaoCasino();
        void gestaoMaquinas();
        void menuCrudMaquina();

        //Quando Run Casino
        void desligarTodasMaquinas();
        void ligarTodasMaquinas();

        //Crud Maquina
        void addMaquina();
        bool removerMaquina(int id_maq);
        bool editarMaquina(int id_maq);

    protected:

    private:
};

#endif // CASINO_H
