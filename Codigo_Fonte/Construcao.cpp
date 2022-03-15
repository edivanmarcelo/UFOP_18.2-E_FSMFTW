#include "Construcao.hpp"
#include <cstring>

using std::string;

void constroiSolucaoParcialmenteGulosaVMP(int qnt)
{
    int v=0, i=0, j=-1, k=0, deposito, EMP, cont=0;
    srand(time(NULL));
    custoSol = 0;

    solucao.resize(i+1);

    k = (rand()%qntD); deposito = k; //Seleciona um deposito aleatorio
    v = (rand()%tiposV); //Seleciona aleatoriamente um veiculo
    solucao[i].veiculo = veiculos[v];


    j = -1; cont = 0; //variavel que auxilia na contagem do tempo
    while(j==-1 && cont<depositos[deposito].durMax && qnt>0){
        j = procuraCMP(k, v, i, EMP); //Procura o cliente mais proximo do deposito
        cont++;
        solucao[i].durTotal++;
    }

    //cout << j-qntD-qntE << endl;
    //cout << EMP-qntD << endl;
    //cout << v << endl;

    while(qnt > 0){
        solucao[i].rota.push_back(k);
        solucao[i].tipo.push_back("d");
        solucao[i].pena.push_back(0);
        solucao[i].tempo.push_back(solucao[i].durTotal);
        solucao[i].bateria.push_back(solucao[i].veiculo.tanque);

        while(qnt>0 && j!=-1){
            solucao[i].demTotal += clientes[j-qntD-qntE].demanda;
            solucao[i].distTotal+= distancias[k][j];
            solucao[i].durTotal += distancias[k][j] + clientes[j-qntD-qntE].durAt;
            solucao[i].tempo.push_back(solucao[i].durTotal);
            solucao[i].rota.push_back(j-qntD-qntE);
            solucao[i].tipo.push_back("c");
            solucao[i].pena.push_back(0);
            solucao[i].veiculo.tanque -= distancias[k][j]/r;
            solucao[i].bateria.push_back(solucao[i].veiculo.tanque);
            atualizaCliente(j); qnt--;

            k = j; cont = 0; j = -1;

            do{
                j = procuraCMP(k, v, i, EMP);
                if(j==-2){
                    adicionaEMP(i, k, EMP, v);
                    k=EMP;
                    j=-1;
                }
                cont++;
                solucao[i].durTotal++;
            }while(j==-1 && solucao[i].durTotal+distancias[k][deposito]<depositos[deposito].durMax && qnt>0);
            //solucao[i].distTotal+= cont;
        }

        if(solucao[i].veiculo.tanque-(distancias[k][deposito]/r) < 0){
            EMP = procuraEMP(k, i, solucao[i].veiculo.tanque);
            adicionaEMP(i, k, EMP, v);
            k=EMP;
        }

        solucao[i].rota.push_back(deposito);
        solucao[i].tipo.push_back("d");
        solucao[i].distTotal += distancias[k][deposito];
        solucao[i].durTotal += distancias[k][deposito];
        if(solucao[i].durTotal>depositos[deposito].durMax)
            solucao[i].pena.push_back(solucao[i].durTotal-depositos[deposito].durMax);
        else
            solucao[i].pena.push_back(0);
        solucao[i].tempo.push_back(solucao[i].durTotal);
        solucao[i].veiculo.tanque -= distancias[k][deposito]/r;
        solucao[i].bateria.push_back(solucao[i].veiculo.tanque);
        solucao[i].custTotal = veiculos[v].custoF + solucao[i].distTotal;
        i++;


        solucao.resize(i+1);

        k = (rand()%qntD);deposito = k;
        v = (rand()%tiposV);
        solucao[i].veiculo = veiculos[v];

        j = -1;cont = 0;
        while(j==-1 && cont<depositos[deposito].durMax){
            j = procuraCMP(k, v, i, EMP);
            while(j==-2){
                if(v<tiposV-1){
                    v++;
                    solucao[i].veiculo = veiculos[v];
                    j = procuraCMP(k, v, i, EMP);
                }
                else{
                    cout << "ERRO" << endl;
                    cin >> cont;
                }
            }
            cont++;
            solucao[i].durTotal++;
        }
    }
    solucao.resize(i);

    for(j=0; j<tiposV; j++){
        solucao.resize(i+1);
        deposito = rand()%qntD;
        solucao[i].veiculo = veiculos[j];

        solucao[i].rota.push_back(deposito);
        solucao[i].tipo.push_back("d");
        solucao[i].pena.push_back(0);
        solucao[i].bateria.push_back(veiculos[j].tanque);
        solucao[i].tempo.push_back(0);

        solucao[i].rota.push_back(deposito);
        solucao[i].tipo.push_back("d");
        solucao[i].pena.push_back(0);
        solucao[i].bateria.push_back(veiculos[j].tanque);
        solucao[i].tempo.push_back(0);
        solucao[i].custTotal = veiculos[j].custoF;
        i++;
    }

    for(i=0; i< solucao.size()-tiposV; i++){
        custoSol += solucao[i].custTotal + solucao[i].pena[solucao[i].pena.size()-1];
    }

}

int adicionaEMP(int i, int k, int EMP, int v){
    double auxCarga;
    solucao[i].veiculo.tanque -= distancias[k][EMP]/r;
    auxCarga = (veiculos[v].tanque - solucao[i].veiculo.tanque)/(g);

    solucao[i].distTotal+= distancias[k][EMP];
    solucao[i].durTotal += distancias[k][EMP] + auxCarga;
    solucao[i].tempo.push_back(solucao[i].durTotal);
    solucao[i].rota.push_back(EMP-qntD);
    solucao[i].tipo.push_back("e");
    solucao[i].veiculo.tanque = veiculos[v].tanque;
    solucao[i].bateria.push_back(solucao[i].veiculo.tanque);

    solucao[i].pena.push_back(0);
}

int  procuraCMP(int linha, int v, int k, int &EMP){
    double menor = -1;
    int indice = -1;
    double auxTempo=0, auxDemanda=0, auxTanque=0;

    for(int i=qntD+qntE; i<qntC+qntD+qntE; i++){
        if(testaCliente(linha, i)){
            auxDemanda = solucao[k].demTotal+clientes[i-qntD-qntE].demanda;
            auxTempo   = solucao[k].durTotal+distancias[linha][i];
            if((auxDemanda<veiculos[v].capacidade) && (auxTempo>=clientes[i-qntD-qntE].iniJanela) && (auxTempo<=clientes[i-qntD-qntE].finJanela)){
                if(menor == -1){
                    menor = distancias[linha][i];
                    indice = i;
                }
                else if(distancias[linha][i] < menor && rand()%3 == 0){
                    menor = distancias[linha][i];
                    indice = i;
                }
            }
        }
    }

    if(indice!=-1){
        auxTanque = solucao[k].veiculo.tanque-(distancias[linha][indice]/r);
        EMP = procuraEMP(indice, k, auxTanque);
        if(EMP == -1){
            indice = -2;
            EMP = procuraEMP(linha, k, solucao[k].veiculo.tanque);
        }
    }

    return indice;
}

int procuraEMP(int linha, int k, double tanque){
    double menor = -1;
    int indice = -1;
    double auxTanque=0;

    for(int i=qntD; i<qntD+qntE; i++){
        auxTanque = tanque - (distancias[linha][i]/r);
        if(auxTanque>=0){
            if(menor == -1){
                menor = distancias[linha][i];
                indice = i;
            }
            else if(distancias[linha][i] < menor){
                menor = distancias[linha][i];
                indice = i;
            }
        }
    }
    return indice;
}

bool testaCliente(int i, int j){
    if(distUsadas[i][j] == -1)
        return false;
    else
        return true;
}

void atualizaCliente(int coluna)
{
    int cont=0;
    for(int i=0; i<qntC+qntD+qntE; i++)
        distUsadas[i][coluna] = -1;
}
