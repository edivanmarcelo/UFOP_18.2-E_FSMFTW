#include"Inicializa.hpp"
#include<cstring>
#include<iomanip>
using std::string;

vector<Cliente> clientes;
vector<Deposito> depositos;
vector<Estacao> estacoes;
vector<Veiculo> veiculos;
vector<double> custosF;
vector<double> custosV;

double** distancias;
double** distUsadas;
vector<Solucao> solucao;
double custoSol;

vector<Solucao> solLinha;
double custSolLinha;

vector<Solucao> solVNS;
double custSolVNS;

vector<Solucao> solVND;
double custSolVND;

vector<Solucao> solSTAR;
double custSolSTAR;

int tiposV; // Qnt de tipos de veiculos
int qntC; // Qnt de clientes
int qntD; // Qnt de depositos
int qntE; // Cont de estações

double r; // fuel consumption rate
double g; // inverse refueling rate
double v; // average Velocity

Veiculo auxVeic; // Auxiliar Veiculo
Cliente auxCliente; // Auxiliar Cliente
Deposito auxDep; // Auxiliar Deposit
Estacao auxEst; // Auxiliar Estação

void lerArq(int qnt, char* instancia){
    int i;
    char chave[4], inicial, tipo[2], enter, lixo[50];
    double x, y, demanda, ijt, fjt, tempoS;
    //string lixo;


    FILE *arq;
    if((arq = fopen(instancia, "r")) == NULL){
        cout << "Erro ao abrir o arquivo!" << endl;
        exit(1);
    }

    fscanf(arq, "%9s %5s %2s %2s %7s %10s %8s %12s %[\n]", lixo, lixo, lixo, lixo, lixo, lixo, lixo, lixo, enter);
    fscanf(arq, "%2s %1s %lf %lf %lf %lf %lf %lf %[\n]", chave, tipo, &x, &y, &demanda, &ijt, &fjt, &tempoS, enter);

    tiposV=0; // Qnt de tipos de veiculos
    qntC=0; // Qnt de clientes
    qntD=0; // Qnt de depositos
    qntE=0;

    while(tipo[0] == 'd'){
        qntD++;
        strcpy(auxDep.chave, chave);
        //auxDep.chave = chave;
        auxDep.coordx = x;
        auxDep.coordy = y;
        auxDep.durMax = fjt;
        depositos.push_back(auxDep);

        /*cout << chave << " ";
        cout << x << " ";
        cout << y << " ";
        cout << fjt << endl;*/

        fscanf(arq, "%4s %1s %lf %lf %lf %lf %lf %lf %[\n]", chave, tipo, &x, &y, &demanda, &ijt, &fjt, &tempoS, enter);
    }
    //cout << endl;

    while(tipo[0] == 'f'){
        qntE++;
        strcpy(auxEst.chave, chave);
        //auxEst.chave = chave;
        auxEst.coordx = x;
        auxEst.coordy = y;
        estacoes.push_back(auxEst);

        /*cout << chave << " ";
        cout << x << " ";
        cout << y << endl;*/

        fscanf(arq, "%4s %1s %lf %lf %lf %lf %lf %lf %[\n]", chave, tipo, &x, &y, &demanda, &ijt, &fjt, &tempoS, enter);
    }
    //cout << endl;

    for(i=0; i<qnt; i++){
        qntC++;
        strcpy(auxCliente.chave, chave);
        //auxCliente.chave = chave;
        auxCliente.coordx = x;
        auxCliente.coordy = y;
        auxCliente.demanda = demanda;
        auxCliente.iniJanela = ijt;
        auxCliente.finJanela = fjt;
        auxCliente.durAt = tempoS;
        clientes.push_back(auxCliente);

        /*cout << chave << " ";
        cout << x << " ";
        cout << y << " ";
        cout << demanda << " ";
        cout << ijt << " ";
        cout << fjt << " ";
        cout << tempoS << endl;*/

        fscanf(arq, "%4s %1s %lf %lf %lf %lf %lf %lf %[\n]", chave, tipo, &x, &y, &demanda, &ijt, &fjt, &tempoS, enter);
    }

    for(int i=0; i<100-qnt; i++)
        fscanf(arq, "%4s %1s %lf %lf %lf %lf %lf %lf %[\n]", chave, tipo, &x, &y, &demanda, &ijt, &fjt, &tempoS, enter);

    //cout << endl;

    int k; // number of vehicle types
    bool aux = true;

    fscanf(arq, "%50[A-Z a-z /] %lf %1s %[\n]", lixo, &x, lixo, enter);
    fscanf(arq, "%50[A-Z a-z /] %lf %1s %[\n]", lixo, &x, lixo, enter);

    while(aux){
    fscanf(arq, "%1s %50[A-Z a-z /] %lf %1s %[\n]", tipo, lixo, &x, lixo, enter);
        switch(tipo[0]){
            case 'r':
                r = x;
                break;
            case 'g':
                g = x;
                break;
            case 'v':
                v = x;
                break;
            case 'k':
                //k = x;
                tiposV = x;
                break;
            default:
                aux = false;
        }
    }
    /*cout << r << endl;
    cout << g << endl;
    cout << v << endl;
    cout << tiposV << endl << endl;*/

    i=0;
    do{
        auxVeic.chave = i+1;
        auxVeic.tanque = x;
        //cout << x << " ";
        fscanf(arq, "%50[A-Z a-z /] %lf %1s %[\n]", lixo, &x, lixo, enter);
        auxVeic.capacidade = x;
        //cout << x << " ";
        fscanf(arq, "%50[A-Z a-z /] %lf %1s %[\n]", lixo, &x, lixo, enter);
        auxVeic.custoF = x;
        //cout << x << endl;
        fscanf(arq, "%50[A-Z a-z /] %lf %1s %[\n]", lixo, &x, lixo, enter);

        veiculos.push_back(auxVeic);
        i++;
    }while(i<tiposV);

    //cout << endl;
    return;
}

void alocarDist(void){
    distancias = new double *[qntC+qntD+qntE];
    for(int i=0; i<qntC+qntD+qntE; i++)
        distancias[i] = new double [qntC+qntD+qntE];

    distUsadas = new double *[qntC+qntD+qntE];
    for(int i=0; i<qntC+qntD+qntE; i++)
        distUsadas[i] = new double [qntC+qntD+qntE];
}

void inicializarDist(void){
    double auxDist=0;

    for(int i=0; i<qntC+qntD+qntE; i++)
        for(int j=0; j<qntC+qntD+qntE; j++)
            distUsadas[i][j] = 0;

    for(int i=0; i<qntC+qntD+qntE; i++){
        distancias[i][i] = -1;
        distUsadas[i][i] = -1;
    }

    for(int i=0; i<qntD; i++){
        for(int j=i+1; j<qntD; j++){
            auxDist =  pow((depositos[i].coordx - depositos[j].coordx), 2);
            auxDist += pow((depositos[i].coordy - depositos[j].coordy), 2);
            distancias[i][j] = sqrt(auxDist);
            distancias[j][i] = sqrt(auxDist);
        }

        for(int j=0; j<qntE; j++){
            auxDist =  pow((depositos[i].coordx - estacoes[j].coordx), 2);
            auxDist += pow((depositos[i].coordy - estacoes[j].coordy), 2);
            distancias[i][j+qntD] = sqrt(auxDist);
            distancias[j+qntD][i] = sqrt(auxDist);
        }

        for(int j=0; j<qntC; j++){
            auxDist =  pow((depositos[i].coordx - clientes[j].coordx), 2);
            auxDist += pow((depositos[i].coordy - clientes[j].coordy), 2);
            distancias[i][j+qntD+qntE] = sqrt(auxDist);
            distancias[j+qntD+qntE][i] = sqrt(auxDist);
        }
    }

    for(int i=0; i<qntE; i++){
        for(int j=i+1; j<qntE; j++){
            auxDist =  pow((estacoes[i].coordx - estacoes[j].coordx), 2);
            auxDist += pow((estacoes[i].coordy - estacoes[j].coordy), 2);
            distancias[i+qntD][j+qntD] = sqrt(auxDist);
            distancias[j+qntD][i+qntD] = sqrt(auxDist);
        }

        for(int j=0; j<qntC; j++){
            auxDist =  pow((estacoes[i].coordx - clientes[j].coordx), 2);
            auxDist += pow((estacoes[i].coordy - clientes[j].coordy), 2);
            distancias[i+qntD][j+qntD+qntE] = sqrt(auxDist);
            distancias[j+qntD+qntE][i+qntD] = sqrt(auxDist);
        }
    }

    for(int i=0; i<qntC; i++){
        for(int j=i+1; j<qntC; j++){
            auxDist =  pow((clientes[i].coordx - clientes[j].coordx), 2);
            auxDist += pow((clientes[i].coordy - clientes[j].coordy), 2);
            distancias[i+qntD+qntE][j+qntD+qntE] = sqrt(auxDist);
            distancias[j+qntD+qntE][i+qntD+qntE] = sqrt(auxDist);
        }
    }

}

void zerarDistUsadas(void){
    for(int i=0; i<qntC+qntD+qntE; i++)
        for(int j=0; j<qntC+qntD+qntE; j++)
            distUsadas[i][j] = 0;

    for(int i=0; i<qntC+qntD+qntE; i++){
        distUsadas[i][i] = -1;
    }
}

void apagarDist(void){
    for(int i=0; i<qntC+qntD+qntE; i++)
        delete [] distancias[i];
    delete [] distancias;

    for(int i=0; i<qntC+qntD+qntE; i++)
        delete [] distUsadas[i];
    delete [] distUsadas;
}


void imprimirDists(void){
    cout << "MATRIZ DE DISTANCIAS: " << endl;
    cout << setw(3)  << fixed << setprecision(0) << "--";
    for(int i=0; i<qntD; i++){
        cout << setw(3)  << fixed << setprecision(0) << i;
    }
    for(int i=0; i<qntE; i++){
        cout << setw(3)  << fixed << setprecision(0) << i;
    }
    for(int i=1; i<=15; i++){
        cout << setw(3)  << fixed << setprecision(0) << i;
    }
    cout << endl << endl;

    for(int i=0; i<qntD; i++){
        cout << setw(3)  << fixed << setprecision(0) << i;
        for(int j=0; j<qntC+qntD+qntE; j++)
            cout << setw(3)  << fixed << setprecision(0) << distancias[i][j];
        cout << endl;
    }
    for(int i=qntD; i<qntD+qntE; i++){
        cout << setw(3)  << fixed << setprecision(0) << i-qntD;
        for(int j=0; j<qntC+qntD+qntE; j++)
            cout << setw(3)  << fixed << setprecision(0) << distancias[i][j];
        cout << endl;
    }
    for(int i=qntD+qntE; i<qntD+qntE+15; i++){
        cout << setw(3)  << fixed << setprecision(0) << i-qntE-qntD+1;
        for(int j=0; j<qntC+qntD+qntE; j++)
            cout << setw(3)  << fixed << setprecision(0) << distancias[i][j];
        cout << endl;
    }
    cout << endl;
    /*for(int i=0; i<qntC+qntD+qntE; i++){
        for(int j=0; j<qntC+qntD+qntE; j++)
            cout << setw(3)  << fixed << setprecision(0) << distUsadas[i][j];
        cout << endl;
    }*/
}


void imprimirSol(void)
{
    cout << "SOLUCAO: " << endl;
    for(int i=0; i<solucao.size()-tiposV; i++){
        /*cout << "ROTA " << i+1 << ": " << endl;
        cout << "CUSTO: " << solucao[i].custTotal << endl;
        cout << "DISTANCIA: " << solucao[i].distTotal << endl;
        cout << "DEMANDA: " << solucao[i].demTotal << endl;
        cout << "DURACAO: " << solucao[i].durTotal << endl;
        cout << "VEICULO: " << solucao[i].veiculo.chave << endl;
        cout << "TANQUE CAP/ATUAL: " << veiculos[solucao[i].veiculo.chave-1].tanque << " / " << solucao[i].veiculo.tanque << endl;*/
        //cout << "ROTA FEITA: " << endl;
        for(int j=0; j<solucao[i].rota.size(); j++){
            if(solucao[i].tipo[j] != "c")
                cout << fixed << setw(5) << solucao[i].tipo[j] << setw(2) << solucao[i].rota[j] << " ";
            else
                cout << fixed << setw(5) << solucao[i].tipo[j] << setw(2) << solucao[i].rota[j]+1 << " ";
        }
        //cout << endl << "BATERIA A CADA CLIENTE: " << endl;
        cout << endl;
        for(int j=0; j<solucao[i].bateria.size(); j++){
            cout << fixed << setw(7) << setprecision(2) << solucao[i].bateria[j] << " ";
        }
        //cout << endl << "TEMPO DE CADA CLIENTE:" << endl;
        cout << endl;
        for(int j=0; j<solucao[i].tempo.size(); j++){
            cout << fixed << setw(7) << setprecision(0) << solucao[i].tempo[j] << " ";
        }
        cout << endl;
        //cout << "ATRASO:" << endl;
        for(int j=0; j<solucao[i].pena.size(); j++){
            cout << fixed << setw(7) << setprecision(2) << solucao[i].pena[j] << " ";
        }
        cout << endl << endl;
    }

    cout << "CUSTO TOTAL: " << custoSol << endl << endl;
}

void verificaSol(int qnt){
    int cont=0;
    int *vetor = new int [qntC];

    for(int i=0; i<solucao.size(); i++)
        for(int j=1; j<solucao[i].rota.size()-1; j++)
            if(solucao[i].tipo[j] == "c")
                vetor[solucao[i].rota[j]] = 1;

    for(int i=0; i<qntC; i++)
        cont += vetor[i];

    //for(int i=0; i<qntC; i++)
    //    cout << i+1 << ": " << vetor[i] << endl;


    if(cont == qnt)
        cout << "CORRETO" << endl;
    else
        cout << "FALTANDO" << endl;

    delete [] vetor;
}
