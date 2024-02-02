#ifndef CASINO_H
#define CASINO_H
#include <iostream>
#include <string>
#include <list>
#include <random>
#include "pugixml.hpp"
#include "Maquina.h"
#include "User.h"

using namespace std;

class Casino
{

    int maxJogadores, jogadoresNoCasino, totalCaixa, totalDinheiroDado;
    int horaAbertura, minutosAbertura, segundosAbertura;
    int horaFecho, minutosFecho, segundosFecho;
    string nome;
    list<Maquina *> LM;
    list<User *> LU;

    public:
        Casino(string _nome);
        virtual ~Casino();
        void Run();

        //Carregar dados atarvés de ficheiros
        void CarregarDados(int _maxJogadores, int _horaAbertura, int _minutosAbertura, int _segundosAbertura,
                           int _horaFecho, int _minutosFecho, int _segundosFecho);
        bool Load(const string& xmlFile);
        bool LoadMaquina(const string& xmlFile);
        bool Add(User *ut);
        bool Add(Maquina *M);
        void Listar(ostream &f);
        void Desligar(int id_maq);
        estadoMaquina Get_Estado(int id_maq);
        int Memoria_Total();
        list<Maquina *> * Listar_Tipo(string Tipo, ostream &f = cout);
        list<string> * Ranking_Dos_Fracos();
        list<Maquina *> * Ranking_Das_Mais_Trabalhadores();
        list<User *> * Jogadores_Mais_Frequentes ();
        list<User *> * Jogadores_Mais_Ganhos ();
        void Relatorio(string fich_xml);
        void SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas);
        void Listar(float X, ostream &f =  cout);

        //get
        int getJogadoresNoCasino() const { return jogadoresNoCasino; }
        int getTotalCaixa(){ return totalCaixa; }
        int getTotalDinheiroDado(){ return totalDinheiroDado; }

        //set
        void setJogadoresNoCasino(int jogadores) { jogadoresNoCasino = jogadores; }
        void setTotalCaixa(int _totalCaixa){ totalCaixa = _totalCaixa; }
        void setTotalDinheiroDado( int _totalDinheiroDado ){ totalDinheiroDado = _totalDinheiroDado; }

    private:

        //Menus
        void Menu();
        void gestaoCasino();
        void gestaoMaquinas();
        void menuCrudMaquina();
        void gestaoUseres();
        //Crud Maquina
        void addMaquina();
        bool removerMaquina(int id_maq);
        bool editarMaquina(int id_maq);
        bool moverMaquina(int id_maq);
        //Metodos secundarios
        void dadosCasino();
        bool entrarJogador();
        void ListarMaquinas();
        string devolveData();
        void removerVizinho(int id_maq);
        bool adicionarVizinho(Maquina *M);
        void mostrarMaquinasAfetadas(list<Maquina *> &maquinasAfetadas);
        void listarRankingMaisTrabalhadores();
        bool compararNjogos(Maquina *maquina1, Maquina *maquina2);
        void listarJogadoresMaisFrequentes();
        void listarJogadoresMaisGanhos();
        //Quando Run Casino
        void desligarTodasMaquinas();
        void ligarTodasMaquinas();
        void maquinaAvariada();
        bool mostrarMaquinas();
        void maquinaGanhos();
        void reparar(int id_maq);
        void registarMaquina();
        string estadoString(estadoMaquina estadoma);
        void ListarMaquinasProbabilidade();
        void avariar(int nMaq);
        void showRankingAvarias();
        void listarTipoMaquina();
        User* userEntraCasino(const string &nomeArquivo);
        int ContarLinhas(const string &nomeArquivo);
        string getNome(){return nome;}

    protected:

};
#endif // CASINO_H
