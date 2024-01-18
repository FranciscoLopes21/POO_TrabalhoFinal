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
    int jogadoresNoCasino;
    int probabilidadeUser;
    int horaAbertura;
    int minutosAbertura;
    int segundosAbertura;
    int horaFecho;
    int minutosFecho;
    int segundosFecho;
    list<Maquina *> LM;
    //list<Maquina *> LM_Avariadas;
    list<User *> LU;
    //list<int> Lt;
    int totalPremios;
    int totalDinheiroDado;



    public:
        Casino(string _nome);
        virtual ~Casino();
        void Run();
        bool entrarJogador();

        void CarregarDados(int _maxJogadores, int _probabilidadeUser,int _horaAbertura, int _minutosAbertura, int _segundosAbertura,
                           int _horaFecho, int _minutosFecho, int _segundosFecho);
        void dadosCasino();
        string getNome(){return nome;}
        void Listar(ostream &f);//compor função


        bool LoadMachinesFromXML(const string& xmlFile);
        bool Add(Maquina *M);
        void ListarMaquinas();


        void maquinaAvariada();
        void maquinaGanhos();
        void reparar(int id_maq);
        void registarMaquina();
        void Desligar(int id_maq);
        estadoMaquina Get_Estado(int id_maq);
        string estadoString(estadoMaquina estadoma);
        void Listar(float X, ostream &f =  cout);
        void ListarMaquinasProbabilidade();

        void avariar(int nMaq);
        list<string> * Ranking_Dos_Fracos();
        void showRankingAvarias();

        void listarTipoMaquina();
        list<Maquina *> * Listar_Tipo(string Tipo, ostream &f = cout);

        User* userEntraCasino(const string &nomeArquivo);
        bool Add(User *ut);
        int ContarLinhas(const string &nomeArquivo);

        //Menus
        void Menu();
        void gestaoCasino();
        void gestaoMaquinas();
        void menuCrudMaquina();
        void gestaoUseres();

        //Quando Run Casino
        void desligarTodasMaquinas();
        void ligarTodasMaquinas();

        //Crud Maquina
        void addMaquina();
        bool removerMaquina(int id_maq);
        bool editarMaquina(int id_maq);
        bool moverMaquina(int id_maq);


        list<Maquina *> * Ranking_Das_Mais_Trabalhadores();
        void listarRankingMaisTrabalhadores();

        bool compararNjogos(Maquina *maquina1, Maquina *maquina2);

        list<User *> * Jogadores_Mais_Frequentes ();
        void listarJogadoresMaisFrequentes();

        list<User *> * Jogadores_Mais_Ganhos ();
        void listarJogadoresMaisGanhos();

        void Relatorio(string fich_xml);

        int getJogadoresNoCasino() const { return jogadoresNoCasino; }
        void setJogadoresNoCasino(int jogadores) { jogadoresNoCasino = jogadores; }

        void SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas);
        void mostrarMaquinasAfetadas(list<Maquina *> &maquinasAfetadas);

        void removerVizinho(int id_maq);
        bool adicionarVizinho(Maquina *M);

        string devolveData();

        int Memoria_Total();


    protected:

    private:
};

#endif // CASINO_H
