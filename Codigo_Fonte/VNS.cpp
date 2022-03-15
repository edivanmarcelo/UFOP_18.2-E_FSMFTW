#include"VNS.hpp"
#include"Construcao.hpp"

void multiStart(int iter, int qnt){
    int contIter=0;

    solLinha.clear();
    solLinha = solucao;
    custSolLinha = custoSol;

    while(contIter < iter){
        contIter++;

        solucao.clear();
        custoSol = 0;
        zerarDistUsadas();
        constroiSolucaoParcialmenteGulosaVMP(qnt);
        VNS(iter);

        if(custoSol < custSolLinha*alfa){
            solLinha.clear();
            solLinha = solucao;
            custSolLinha = custoSol;
            contIter=0;
        }
    }

    solucao.clear();
    solucao = solLinha;
    custoSol = custSolLinha;
}

void vnd(int iter){
    int k=0, aux=0, mov[NV], x, y;
    int contIter=0;
    srand(time(NULL));

    for(int i=0; i< NV; i++)
        mov[i] = i;

    for(int i=0; i<NV; i++){
        x = rand()%(NV-1)+1;
        y = rand()%(NV-1)+1;
        aux = mov[x];
        mov[x] = mov[y];
        mov[y] = aux;
    }

    while(k<NV){
        while(contIter < iter){
            solVND.clear();
            solVND = solVNS;
            custSolVND = custSolVNS;

            melhorVizinhoVND(mov[k]);

            if(custSolVND<custSolVNS*alfa){
                solVNS.clear();
                solVNS = solVND;
                custSolVNS= custSolVND;
                k=0; contIter=0;
            }
            else
                contIter++;
        }
        k++;
    }
}

void VNS(int iter){
    int contIter=0, k=0, aux=0, mov[NV_VNS], x, y;
    srand(time(NULL));

    for(int i=0; i< NV_VNS; i++)
        mov[i] = i;

    for(int i=0; i< NV_VNS; i++){
        x = rand()%NV_VNS;
        y = rand()%NV_VNS;
        aux = mov[x];
        mov[x] = mov[y];
        mov[y] = aux;
    }

    while(k<NV_VNS){
        while(contIter < iter){
            solVNS.clear();
            solVNS = solucao;
            custSolVNS = custoSol;

            vizinhoQualquerVNS(mov[k]);
            vnd(iter);

            if(custSolVNS<custoSol*alfa){
                solucao.clear();
                solucao = solVNS;
                custoSol = custSolVNS;
                k=0; contIter=0;
            }
            else
                contIter++;
        }
        k++;
    }
}

void melhorVizinhoVND(int area){
    switch(area){
        case 3:
            trocaIntra_VND();
            break;
        case 1:
            realocaIntra_VND();
            break;
        case 2:
            trocaInter_VND();
            break;
        case 0:
            realocaInter_VND();
    }
}

void realocaIntra_VND(void){
    int iSol=0, i=1, j=2;
    int chave;string tipo;

    vector<Solucao> auxSolucao;
    auxSolucao.clear();
    auxSolucao = solVND;
    double auxCusto = custSolVND;

    solSTAR.clear();
    solSTAR = solVND;
    custSolSTAR = custSolVND;

    if(j>=solVND[iSol].rota.size()-1)
        return;

    for(iSol=0; iSol<solVND.size()-tiposV; iSol++){

        while(i<solVND[iSol].rota.size()-2){
            while(j<solVND[iSol].rota.size()-1){

                chave = solVND[iSol].rota[i];
                tipo  = solVND[iSol].tipo[i];

                for(int k=i; k<j; k++){
                    solVND[iSol].rota[k] = solVND[iSol].rota[k+1];
                    solVND[iSol].tipo[k] = solVND[iSol].tipo[k+1];
                }
                solVND[iSol].rota[j] = chave;
                solVND[iSol].tipo[j] = tipo;

                alteraSolRealoc_VND(iSol);
                if(custSolVND < custSolSTAR){
                    solSTAR.clear();
                    solSTAR = solVND;
                    custSolSTAR = custSolVND;
                }
                j++;
                solVND.clear();
                solVND = auxSolucao;
                custSolVND = auxCusto;
            }
            if(custSolSTAR < auxCusto){
                auxSolucao.clear();
                auxSolucao = solSTAR;
                auxCusto = custSolSTAR;
                i=1; j=2;
            }
            else{
                i++; j=i+1;
            }
        }
    }
    solVND.clear();
    solVND = auxSolucao;
    custSolVND = auxCusto;
}

void trocaIntra_VND(void){
    int iSol, cont;
    int aux;
    bool continua;
    srand(time(NULL));

    solSTAR.clear();
    solSTAR = solVND;
    custSolSTAR = custSolVND;

    vector<Solucao> auxSolucao;
    auxSolucao.clear();
    auxSolucao = solVND;
    double auxCusto = custSolVND;

    do{
        continua = false;
        for(iSol=0; iSol<solVND.size()-tiposV; iSol++){
            for(int i=1; i<solVND[iSol].rota.size()-2; i++){

                cont=1;
                while(solVND[iSol].tipo[i] == "e" && i<solVND[iSol].rota.size()-2)
                    i++;
                while(solVND[iSol].tipo[i+cont] == "e" && i+cont<solVND[iSol].rota.size()-1)
                    cont++;
                if(i+cont < solVND[iSol].rota.size()-1){

                    aux = solVND[iSol].rota[i];
                    solVND[iSol].rota[i] = solVND[iSol].rota[i+cont];
                    solVND[iSol].rota[i+cont] = aux;

                    alteraSolTroca_VND(iSol);

                    if(custSolVND < custSolSTAR){
                        solSTAR.clear();
                        solSTAR = solVND;
                        custSolSTAR = custSolVND;
                    }
                    solVND.clear();
                    solVND = auxSolucao;
                    custSolVND = auxCusto;
                }
            }
        }
        if(custSolSTAR<auxCusto){
            auxSolucao.clear();
            auxSolucao = solSTAR;
            auxCusto = custSolSTAR;
            continua = true;
        }
    }while(continua);

    solVND.clear();
    solVND = auxSolucao;
    custSolVND = auxCusto;
}

void realocaInter_VND(void){
    int iSol1, iSol2, i=1, j=1;
    int chave1;string tipo1;
    int chave2;string tipo2;
    bool continua = true, n_para = true;

    int vdd1=0, vdd2=0;
    double rota1, rota2;

    vector<Solucao> auxSolucao;
    auxSolucao.clear();
    auxSolucao = solVND;
    double auxCusto = custSolVND;

    solSTAR.clear();
    solSTAR = solVND;
    custSolSTAR = custSolVND;

    if(solVND.size()-tiposV < 2)
        return;

    iSol1 = rand()%(solVND.size()-tiposV);
    do{
        iSol2 = rand()%(solVND.size()-tiposV);
    }while(iSol1 == iSol2);

    while(continua && n_para){
        continua = false;
        while(i<solVND[iSol1].rota.size()-1){
            while(j<solVND[iSol2].rota.size()-1){

                chave1 = solVND[iSol1].rota[i];
                tipo1  = solVND[iSol1].tipo[i];
                for(int k=i; k<solVND[iSol1].rota.size()-1; k++){
                    solVND[iSol1].rota[k] = solVND[iSol1].rota[k+1];
                    solVND[iSol1].tipo[k] = solVND[iSol1].tipo[k+1];
                }
                solVND[iSol1].rota.resize(solVND[iSol1].rota.size()-1);
                solVND[iSol1].tipo.resize(solVND[iSol1].tipo.size()-1);
                solVND[iSol1].tempo.resize(solVND[iSol1].tempo.size()-1);
                solVND[iSol1].bateria.resize(solVND[iSol1].bateria.size()-1);
                solVND[iSol1].pena.resize(solVND[iSol1].pena.size()-1);


                for(int k=j; k<solVND[iSol2].rota.size(); k++){
                    chave2 = solVND[iSol2].rota[k];
                    tipo2  = solVND[iSol2].tipo[k];

                    solVND[iSol2].rota[k] = chave1;
                    solVND[iSol2].tipo[k] = tipo1;

                    chave1 = chave2;
                    tipo1 = tipo2;
                }
                solVND[iSol2].rota.push_back(chave1);
                solVND[iSol2].tipo.push_back(tipo1);
                solVND[iSol2].tempo.push_back(0);
                solVND[iSol2].bateria.push_back(0);
                solVND[iSol2].pena.push_back(0);

                vdd2 = alteraSolRealoc_VND(iSol2);
                rota2 = solVND[iSol2].custTotal;

                if(solVND[iSol1].rota.size() <= 2){
                    for(int k=iSol1; k<solVND.size()-1; k++){
                        solVND[k] = solVND[k+1];
                    }
                    vdd1 = 1;
                    rota1=0;
                    solVND.resize(solVND.size()-1);
                    n_para = false;
                }else{
                    vdd1 = alteraSolRealoc_VND(iSol1);
                    rota1 = solVND[iSol1].custTotal;
                }

                if(vdd1!= -1 && vdd2 !=-1){
                    custSolVND = auxCusto - (auxSolucao[iSol1].custTotal+auxSolucao[iSol2].custTotal);
                    custSolVND = custSolVND + (rota1+rota2);

                    if(custSolVND < custSolSTAR){
                        solSTAR.clear();
                        solSTAR = solVND;
                        custSolSTAR = custSolVND;
                    }
                }
                j++;
                solVND.clear();
                solVND = auxSolucao;
                custSolVND = auxCusto;
            }
                i++; j=1;
        }
        if(custSolSTAR < auxCusto){
            auxSolucao.clear();
            auxSolucao = solSTAR;
            auxCusto = custSolSTAR;
            i=1; j=1;
            continua = true;

        }
    }
    solVND.clear();
    solVND = auxSolucao;
    custSolVND = auxCusto;
}

void trocaInter_VND(void){
    int iSol1, iSol2, cont;
    int aux, vdd1=0, vdd2=0;
    bool continua=true;
    srand(time(NULL));

    iSol1 = rand()%(solVND.size()-tiposV);
    do{
        iSol2 = rand()%(solVND.size()-tiposV);
    }while(iSol1 == iSol2);

    solSTAR.clear();
    solSTAR = solVND;
    custSolSTAR = custSolVND;

    vector<Solucao> auxSolucao;
    auxSolucao.clear();
    auxSolucao = solVND;
    double auxCusto = custSolVND;

    while(continua){
        continua = false;
        for(int i=1; i<solVND[iSol1].rota.size()-1; i++){
            for(int j=1; j<solVND[iSol2].rota.size()-1; j++){

                while(solVND[iSol1].tipo[i] == "e" && i<solVND[iSol1].rota.size()-1)
                    i++;
                while(solVND[iSol2].tipo[j] == "e" && j<solVND[iSol2].rota.size()-1)
                    j++;
                if(i<solVND[iSol1].rota.size()-1 && j<solVND[iSol2].rota.size()-1){

                    aux = solVND[iSol1].rota[i];
                    solVND[iSol1].rota[i] = solVND[iSol2].rota[j];
                    solVND[iSol2].rota[j] = aux;

                    vdd1 = alteraSolTroca_VND(iSol1);
                    vdd2 = alteraSolTroca_VND(iSol2);
                    if(vdd1!= -1 && vdd2 !=-1){
                        custSolVND = auxCusto - (auxSolucao[iSol1].custTotal+auxSolucao[iSol2].custTotal);
                        custSolVND = custSolVND + (solVND[iSol1].custTotal+solVND[iSol2].custTotal);

                        if(custSolVND < custSolSTAR){
                            solSTAR.clear();
                            solSTAR = solVND;
                            custSolSTAR = custSolVND;
                        }
                    }
                    solVND.clear();
                    solVND = auxSolucao;
                    custSolVND = auxCusto;
                }
            }
        }
        if(custSolSTAR<auxCusto){
            auxSolucao.clear();
            auxSolucao = solSTAR;
            auxCusto = custSolSTAR;
            continua = true;
        }
    }

    solVND.clear();
    solVND = auxSolucao;
    custSolVND = auxCusto;
}

int alteraSolTroca_VND(int iSol){

    int chaveAnt=solVND[iSol].rota[0], chave = solVND[iSol].rota[1];
    int v=solVND[iSol].veiculo.chave-1;
    int k=0;
    double espera, pena, auxCusto;
    auxCusto = custSolVND-solVND[iSol].custTotal;

    solVND[iSol].distTotal= 0;
    solVND[iSol].custTotal= 0;
    solVND[iSol].demTotal = 0;
    solVND[iSol].durTotal = 0;
    solVND[iSol].tempo[k] = 0;
    solVND[iSol].bateria[k] = veiculos[v].tanque;
    solVND[iSol].veiculo.tanque = veiculos[v].tanque;

    espera = clientes[chave].iniJanela - (solVND[iSol].durTotal + distancias[chaveAnt][chave+qntD+qntE]);
    if(espera > 0){
        solVND[iSol].durTotal += espera;
        solVND[iSol].tempo[k]  = espera;
    }

    for(k=1; k<solVND[iSol].rota.size(); k++){
        if(solVND[iSol].tipo[k] == "c"){
            chave += qntD+qntE;
            solVND[iSol].veiculo.tanque -= (distancias[chaveAnt][chave]/r);
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            espera = clientes[chave-qntD-qntE].iniJanela - (solVND[iSol].durTotal + distancias[chaveAnt][chave]);
            pena   = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - clientes[chave-qntD-qntE].finJanela;

            solVND[iSol].distTotal += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]    = 0;
            solVND[iSol].demTotal  += clientes[chave-qntD-qntE].demanda;
            solVND[iSol].durTotal  += distancias[chaveAnt][chave] + clientes[chave-qntD-qntE].durAt;

            if(espera > 0){
                solVND[iSol].durTotal += espera;
            }else if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].tempo[k]   = solVND[iSol].durTotal;
            solVND[iSol].bateria[k] = solVND[iSol].veiculo.tanque;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }

        else if(solVND[iSol].tipo[k] == "e"){
            chave += qntD;
            solVND[iSol].veiculo.tanque -= distancias[chaveAnt][chave]/r;
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            pena = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - depositos[0].durMax;
            solVND[iSol].distTotal     += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]        = 0;
            solVND[iSol].durTotal      += distancias[chaveAnt][chave] + (veiculos[v].tanque - solVND[iSol].veiculo.tanque)/(g);
            solVND[iSol].tempo[k]       = solVND[iSol].durTotal;
            solVND[iSol].veiculo.tanque = veiculos[v].tanque;
            solVND[iSol].bateria[k]     = veiculos[v].tanque;

            if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }

        else if(solVND[iSol].tipo[k] == "d"){
            solVND[iSol].veiculo.tanque -= distancias[chaveAnt][chave]/r;
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            pena   = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - depositos[chave].durMax;
            solVND[iSol].distTotal += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]    = 0;
            solVND[iSol].durTotal  += distancias[chaveAnt][chave];
            solVND[iSol].tempo[k]   = solVND[iSol].durTotal;
            solVND[iSol].bateria[k] = solVND[iSol].veiculo.tanque;

            if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }
        chaveAnt = chave;
        chave    = solVND[iSol].rota[k+1];
    }
    solVND[iSol].custTotal += veiculos[v].custoF + solVND[iSol].distTotal;
    custSolVND = auxCusto + solVND[iSol].custTotal;
    return 1;
}

int alteraSolRealoc_VND(int iSol){

    int chaveAnt=solVND[iSol].rota[0], chave = solVND[iSol].rota[1];
    int v=solVND[iSol].veiculo.chave-1;
    int k=0;
    double espera, pena, auxCusto;
    auxCusto = custSolVND-solVND[iSol].custTotal;

    solVND[iSol].distTotal= 0;
    solVND[iSol].custTotal= 0;
    solVND[iSol].demTotal = 0;
    solVND[iSol].durTotal = 0;
    solVND[iSol].tempo[k] = 0;
    solVND[iSol].bateria[k] = veiculos[v].tanque;
    solVND[iSol].veiculo.tanque = veiculos[v].tanque;

    espera = clientes[chave].iniJanela - (solVND[iSol].durTotal + distancias[chaveAnt][chave+qntD+qntE]);
    if(espera > 0){
        solVND[iSol].durTotal += espera;
        solVND[iSol].tempo[k]  = espera;
    }

    for(k=1; k<solVND[iSol].rota.size(); k++){
        if(solVND[iSol].tipo[k] == "c"){
            chave += qntD+qntE;
            solVND[iSol].veiculo.tanque -= (distancias[chaveAnt][chave]/r);
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            espera = clientes[chave-qntD-qntE].iniJanela - (solVND[iSol].durTotal + distancias[chaveAnt][chave]);
            pena   = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - clientes[chave-qntD-qntE].finJanela;

            solVND[iSol].distTotal += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]    = 0;
            solVND[iSol].demTotal  += clientes[chave-qntD-qntE].demanda;
            solVND[iSol].durTotal  += distancias[chaveAnt][chave] + clientes[chave-qntD-qntE].durAt;

            if(espera > 0){
                solVND[iSol].durTotal += espera;
            }else if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].tempo[k]   = solVND[iSol].durTotal;
            solVND[iSol].bateria[k] = solVND[iSol].veiculo.tanque;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }

        else if(solVND[iSol].tipo[k] == "e"){
            chave += qntD;
            solVND[iSol].veiculo.tanque -= distancias[chaveAnt][chave]/r;
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            pena = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - depositos[0].durMax;
            solVND[iSol].distTotal     += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]        = 0;
            solVND[iSol].durTotal      += distancias[chaveAnt][chave] + (veiculos[v].tanque - solVND[iSol].veiculo.tanque)/(g);
            solVND[iSol].tempo[k]       = solVND[iSol].durTotal;
            solVND[iSol].veiculo.tanque = veiculos[v].tanque;
            solVND[iSol].bateria[k]     = veiculos[v].tanque;

            if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }

        else if(solVND[iSol].tipo[k] == "d"){
            solVND[iSol].veiculo.tanque -= distancias[chaveAnt][chave]/r;
            if(solVND[iSol].veiculo.tanque < 0)
                return -1;

            pena   = (solVND[iSol].durTotal + distancias[chaveAnt][chave]) - depositos[chave].durMax;
            solVND[iSol].distTotal += distancias[chaveAnt][chave];
            solVND[iSol].pena[k]    = 0;
            solVND[iSol].durTotal  += distancias[chaveAnt][chave];
            solVND[iSol].tempo[k]   = solVND[iSol].durTotal;
            solVND[iSol].bateria[k] = solVND[iSol].veiculo.tanque;

            if(pena > 0)
                solVND[iSol].pena[k] = pena;

            solVND[iSol].custTotal += solVND[iSol].pena[k];
        }
        chaveAnt = chave;
        chave    = solVND[iSol].rota[k+1];
    }
    solVND[iSol].custTotal += veiculos[v].custoF + solVND[iSol].distTotal;
    custSolVND = auxCusto + solVND[iSol].custTotal;
    return 1;
}

void vizinhoQualquerVNS(int area){
    switch(area){
    case 0:
        realocaIntra_VNS();
        break;
    case 1:
        trocaIntra_VNS();
        break;
    case 2:
        realocaInter_VNS();
        break;
    case 3:
        trocaInter_VNS();
        break;
    }
}

void trocaIntra_VNS(void){
    int iSol, i, j=0, aux, vdd=-1;
    srand(time(NULL));

    iSol = rand()%(solVNS.size()-tiposV);
    do{
        i = rand()%solVNS[iSol].rota.size();
    }while(solVNS[iSol].tipo[i] != "c");

    do{
        do{
            j++;
        }while((solVNS[iSol].tipo[j] != "c" || j==i) && (j<solVNS[iSol].rota.size()-1));

        if(j<solVNS[iSol].rota.size()-1){
            solVND.clear();
            solVND = solVNS;
            custSolVND = custSolVNS;

            aux = solVND[iSol].rota[i];
            solVND[iSol].rota[i] = solVND[iSol].rota[j];
            solVND[iSol].rota[j] = aux;

            vdd = alteraSolTroca_VND(iSol);

            if(vdd == 1){
                solVNS.clear();
                solVNS = solVND;
                custSolVNS = custSolVND;
                if(rand()%2 == 0)
                    vdd = -1;
            }
        }
        else{
            iSol = rand()%(solVNS.size()-tiposV);
            do{
                i = rand()%solVNS[iSol].rota.size();
            }while(solVNS[iSol].tipo[i] != "c");
            j=0;
        }
    }while(vdd == -1);
}

void realocaIntra_VNS(void){
    int iSol, i, j=0, aux, vdd=-1;
    int chave; string tipo;
    srand(time(NULL));

    do{
        iSol = rand()%(solVNS.size()-tiposV);
    }while(solVNS[iSol].rota.size()<2);

    do{
        i = rand()%solVNS[iSol].rota.size();
        j=i;
    }while(solVNS[iSol].tipo[i] != "c");

    do{
        j++;
        if(j<solVNS[iSol].rota.size()-1){
            solVND.clear();
            solVND = solVNS;
            custSolVND = custSolVNS;

            chave = solVND[iSol].rota[i];
            tipo  = solVND[iSol].tipo[i];

            for(int k=i; k<j; k++){
                solVND[iSol].rota[k] = solVND[iSol].rota[k+1];
                solVND[iSol].tipo[k] = solVND[iSol].tipo[k+1];
            }
            solVND[iSol].rota[j] = chave;
            solVND[iSol].tipo[j] = tipo;

            vdd = alteraSolRealoc_VND(iSol);

            if(vdd == 1){
                solVNS.clear();
                solVNS = solVND;
                custSolVNS = custSolVND;
                if(rand()%2 == 0)
                    vdd = -1;
            }
        }
        else{
            do{
                iSol = rand()%(solVNS.size()-tiposV);
            }while(solVNS[iSol].rota.size()<2);

            do{
                i = rand()%solVNS[iSol].rota.size();
                j=i;
            }while(solVNS[iSol].tipo[i] != "c");
        }
    }while(vdd == -1);
}

void trocaInter_VNS(void){
    int iSol1, iSol2;
    int i=0, j=0;
    int aux, vdd1=-1, vdd2=-1;
    srand(time(NULL));

    iSol1 = rand()%(solVNS.size()-tiposV);
    do{
        iSol2 = rand()%(solVNS.size()-tiposV);
    }while(iSol1 == iSol2);

    do{
        i = rand()%solVNS[iSol1].rota.size();
    }while(solVNS[iSol1].tipo[i] != "c");

    do{
        do{
            j++;
        }while((solVNS[iSol2].tipo[j] != "c") && (j<solVNS[iSol2].rota.size()-1));

        if(j<solVNS[iSol2].rota.size()-1){
            solVND.clear();
            solVND = solVNS;
            custSolVND = custSolVNS;

            aux = solVND[iSol1].rota[i];
            solVND[iSol1].rota[i] = solVND[iSol2].rota[j];
            solVND[iSol2].rota[j] = aux;

            vdd1 = alteraSolTroca_VND(iSol1);
            vdd2 = alteraSolTroca_VND(iSol2);

            if(vdd1==1 && vdd2==1){
                custSolVND = custSolVNS - (solVNS[iSol1].custTotal+solVNS[iSol2].custTotal);
                custSolVND = custSolVND + (solVND[iSol1].custTotal+solVND[iSol2].custTotal);

                solVNS.clear();
                solVNS = solVND;
                custSolVNS = custSolVND;
                if(rand()%2 == 0)
                    vdd1 = -1;
            }
        }
        else{
            iSol1 = rand()%(solVNS.size()-tiposV);
            do{
                iSol2 = rand()%(solVNS.size()-tiposV);
            }while(iSol1 == iSol2);
            do{
                i = rand()%solVNS[iSol1].rota.size();
            }while(solVNS[iSol1].tipo[i] != "c");
            j=0;
        }
    }while(vdd1 == -1 || vdd2 == -1);

}

void realocaInter_VNS(void){
    int iSol1, iSol2, i=1, j=1;
    int chave1;string tipo1;
    int chave2;string tipo2;

    int vdd1=0, vdd2=0;
    double rota1, rota2;

    solVND.clear();
    solVND = solVNS;
    custSolVND = custSolVNS;

    if(solVND.size()-tiposV < 2)
        return;

    iSol1 = rand()%(solVND.size()-tiposV);
    do{
        iSol2 = rand()%(solVND.size()-tiposV);
    }while(iSol1 == iSol2);

    while(i<solVND[iSol1].rota.size()-1){
        while(j<solVND[iSol2].rota.size()-1){

            chave1 = solVND[iSol1].rota[i];
            tipo1  = solVND[iSol1].tipo[i];
            for(int k=i; k<solVND[iSol1].rota.size()-1; k++){
                solVND[iSol1].rota[k] = solVND[iSol1].rota[k+1];
                solVND[iSol1].tipo[k] = solVND[iSol1].tipo[k+1];
            }
            solVND[iSol1].rota.resize(solVND[iSol1].rota.size()-1);
            solVND[iSol1].tipo.resize(solVND[iSol1].tipo.size()-1);
            solVND[iSol1].tempo.resize(solVND[iSol1].tempo.size()-1);
            solVND[iSol1].bateria.resize(solVND[iSol1].bateria.size()-1);
            solVND[iSol1].pena.resize(solVND[iSol1].pena.size()-1);


            for(int k=j; k<solVND[iSol2].rota.size(); k++){
                chave2 = solVND[iSol2].rota[k];
                tipo2  = solVND[iSol2].tipo[k];

                solVND[iSol2].rota[k] = chave1;
                solVND[iSol2].tipo[k] = tipo1;

                chave1 = chave2;
                tipo1 = tipo2;
            }
            solVND[iSol2].rota.push_back(chave1);
            solVND[iSol2].tipo.push_back(tipo1);
            solVND[iSol2].tempo.push_back(0);
            solVND[iSol2].bateria.push_back(0);
            solVND[iSol2].pena.push_back(0);


            vdd2 = alteraSolRealoc_VND(iSol2);
            rota2 = solVND[iSol2].custTotal;

            if(solVND[iSol1].rota.size() <= 2){
                for(int k=iSol1; k<solVND.size()-1; k++){
                    solVND[k] = solVND[k+1];
                }
                vdd1 = 1;
                rota1=0;
                solVND.resize(solVND.size()-1);
            }else{
                vdd1 = alteraSolRealoc_VND(iSol1);
                rota1 = solVND[iSol1].custTotal;
            }

            if(vdd1!= -1 && vdd2 !=-1){
                custSolVND = custSolVNS - (solVNS[iSol1].custTotal+solVNS[iSol2].custTotal);
                custSolVND = custSolVND + (rota1+rota2);

                 if(rand()%2 == 0){
                    solVNS.clear();
                    solVNS = solVND;
                    custSolVNS = custSolVND;
                    return;
                 }
            }
            j++;
            solVND.clear();
            solVND = solVNS;
            custSolVND = custSolVNS;
        }
            i++; j=1;
    }
}

void novaRota_VNS(void){

}
