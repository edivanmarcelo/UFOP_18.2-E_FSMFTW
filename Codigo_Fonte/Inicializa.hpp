#ifndef Inicializa_hpp
#define Inicializa_hpp

#include<iostream>
#include<cstdlib>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip>
#include<iterator>
#include<cmath>
#include<cstring>
#include <time.h>
using namespace std;
using std::string;

struct Cliente{
    char chave[4];
    double coordx, coordy;
    double demanda;
    double iniJanela, finJanela;
    double durAt;

};

struct Veiculo{
    int chave;
    double tanque;
    double capacidade;
    double custoF;
};

struct Deposito{
    char chave[2];
    double coordx;
    double coordy;
    double durMax;
};

struct Estacao{
    char chave[3];
    double coordx;
    double coordy;
};

struct Solucao{
    double distTotal=0;
    double custTotal=0;
    double demTotal=0;
    double durTotal=0;
    Veiculo veiculo;

    vector <int> rota;
    vector <string> tipo;
    vector <double> bateria;
    vector <double> tempo;
    vector <double> pena;
};

extern vector<Cliente> clientes;
extern vector<Deposito> depositos;
extern vector<Estacao> estacoes;
extern vector<Veiculo> veiculos;
extern vector<double> custosF;

extern double** distancias;
extern double** distUsadas;
extern vector<Solucao> solucao;
extern double custoSol;

extern vector<Solucao> solLinha;
extern double custSolLinha;

extern vector<Solucao> solVNS;
extern double custSolVNS;

extern vector<Solucao> solVND;
extern double custSolVND;

extern vector<Solucao> solSTAR;
extern double custSolSTAR;

extern int tiposV; // Cont de tipos de veiculos
extern int qntC; // Cont de clientes
extern int qntD; // Cont de depositos
extern int qntE; // Cont de estações

extern Veiculo auxVeic; // Auxiliar Veiculo
extern Cliente auxCliente; // Auxiliar Cliente
extern Deposito auxDep; // Auxiliar Deposito
extern Estacao auxEst; // Auxiliar Estação

extern double r; // fuel consumption rate
extern double g; // inverse refueling rate
extern double v; // average Velocity


void lerArq(int qnt, char* instancia);

void alocarDist(void);
void inicializarDist(void);
void zerarDistUsadas(void);
void apagarDist(void);
void imprimirDists(void);

void imprimirSol(void);
void verificaSol(int qnt);

#endif

