/*Bugs:
-----------------------------------------------------------------------------
Observacoes:
-Uma IDEIA (não sei o quão palpável ela será) seria, talvez, gravar todas as combinações de slots possíveis
para deixar ao usuario uma gama legal de combinações de correções de prova disponíveis.
-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

void variancia(int a, int b, int c, int alu, int mon, int mona, int monb, int monc, int *monabc){

    int vmin=INT_MAX;
    double v, media;

    media = alu/mon;

    for(mona=1; mona<=mon-2; mona++){
        for(monb=1; monb<=mon-mona-1; monb++){
            monc = mon - mona - monb;
            //Calculo de variancia
            v = (pow(((a/mona)-media), 2) + pow(((b/monb)-media), 2) + pow(((c/monc)-media), 2))/3;
            //Verifica sempre se a variancia é pequena o suficiente
            if(v<vmin){
                vmin = v;
                monabc[0] = mona;
                monabc[1] = monb;
                monabc[2] = monc;
            }
        }
    }
    return;
}

void slots(int a, int b, int c, int *monabc, int **slot[3], int *n){
    int soma, i, j, x;
    double med[3];

    //Slots de espera de monitores (ficam vazios à espera de um monitor que se "encaixe" em suas condicoes, que sera o próximo passo)
    for(i=0; i<3; i++){
        int soma;

        soma=0;
        x=0;

        //Testa de qual prova estamos falando
        if(i==0)
            med[i] = a/(double)monabc[i];
        else if(i==1)
            med[i] = b/(double)monabc[i];
        else
            med[i] = c/(double)monabc[i];

        for(j=0; j<monabc[i]; j++){
                slot[i][j]= (int)floor(med[i]);
                soma += slot[i][j];
        }
        //Fica somando um até que o valor, que foi arredondado pra baixo, se torne igual ao inteiro que representa o numero de monitores corretores
        while(soma<n[i]){
                slot[i][x]++;
                soma++;
                x++;
        }
    }
    return;
}

int main(){

    //Variaveis de contagem
    int a=0, b=0, c=0, i=0, j=0, k=0, n[i], t, x=0, y=0, z=0, mon=0, alu=0, flag=0, mflag[50]={0}, aflag[100]={0},contmon[50]={0}, monabc[3]={0}, fez[3][100], slot[3][50]={0}, monal3[50][3];
    //Variaveis para cálculo de variância, média de alunos por monitor em cada prova e slots.
    double mona=0, monb=0, monc=0, med[3];
    //Variavel de escolha
    char opt;
    //Matrizes de texto
    char monitores[50][50], alunos[100][50], monal[50][50], monal2[50][50], prova[100][2], vprova[100], A[100][50], B[100][50], C[100][50];
    //A FODEROSA MATRIZ QUEM-CORRIGE-QUEM, MOTIVO DESTE PROGRAMA EXISTIR!
    char qcq[50][100][50];

    printf("Digite o n%cmero de grupos de monitoria que ir%c corrigir a(s) provas:", 163, 160);
    scanf("%d", &mon);
    system("cls");//Tela limpa

    //Etapa de coleta de dados dos monitores
    for(i=0; i<mon; i++){
        do{
            printf("Digite o nome do(s) monitores do grupo %d:", i+1);
            fflush(stdin);
            fgets(monitores[i], 50, stdin);
            monitores[i][strlen(monitores[i])-1] = '\0';//Mata \n
            system("cls");//Tela limpa

            //Minusculando os monitores
            while(monitores[i][j]!= '\0'){
            monitores[i][j] = tolower(monitores[i][j]);
            j++;
            }

            //Zerando a variavel para reciclagem
            j=0;

            //Verifica que o monitor digitado não consta na memoria (não é repetido)
            for(k=0; k<i; k++){
                if(strcmp(monitores[i], monitores[k])==0){
                    flag++;
                    k = (i-1);
                }else if(k==(i-1) && strcmp(monitores[i], monitores[k])!=0){
                    flag=0;
                }
            }
            if(flag!=0){
                system("cls");//Tela limpa
                printf("Aviso! O monitor %s ja foi registrado!\n", monitores[i]);
            }
            }while(flag!=0);
    }

    //Zerando as variaveis para reciclagem
    i=0;
    j=0;
    k=0;

    //Etapa de coleta de dados dos alunos
    do{

        //Coleta de nome
        printf("Digite o nome ou cartao UFRGS do(a) aluno(a) %d:", i+1);
        fflush(stdin);
        fgets(alunos[i], 50, stdin);
        alunos[i][strlen(alunos[i])-1] = '\0';//Mata \n

        //Coleta de prova
        do{
            printf("\nDigite qual prova %s realizou (A, B ou C):", alunos[i]);
            fflush(stdin);
            fgets(prova[i], 2, stdin);

            //Minusculando a prova
            prova[i][0] = tolower(prova[i][0]);

            if(prova[i][0]!='a' && prova[i][0]!='b' && prova[i][0]!='c'){
                system("cls");//Tela limpa
                printf("Invalido! Digite somente A, B ou C.");
            }

            }while(prova[i][0]!='a' && prova[i][0]!='b' && prova[i][0]!='c');

        //Simplificando as provas em um vetor unidimensional
        vprova[i] = prova[i][0];

        //Coleta de monitor
        do{
            printf("\nDigite o monitor de %s:", alunos[i]);
            fflush(stdin);
            fgets(monal[i], 50, stdin);
            monal[i][strlen(monal[i])-1] = '\0';

            //Minusculando os monitores (coletados no campo de alunos)
            while(monal[i][j]!= '\0'){
                monal[i][j] = tolower(monal[i][j]);
                j++;
            }

            //Zerando variavel para reciclagem
            j=0;

            //Verifica que o monitor digitado consta na memoria
            for(k=0; k<mon; k++){
                if(strcmp(monal[i], monitores[k])!=0){
                    flag++;
                    monal3[k][vprova[i]-97]++;
                }else{
                    flag=0;
                    k=(mon-1);
                }
            }
            if(flag!=0){
                system("cls");//Tela limpa
                printf("Aviso! O monitor %s n%co existe!", monal[i], 198);
                }
        }while(flag!=0);

        //Zerando variável para reciclagem
        k=0;

        system("cls");//Tela limpa

        //Algoritmo de repetição controlada pelo usuário
        printf("=====================O QUE FAZER AGORA?=====================");
        printf("\nDeseja inserir mais um aluno? Digite 's'.\nDeseja reescrever os dados do aluno %d? Digite 'v'.", i+1);
        if(i>0){
        printf("\nDeseja excluir o aluno digitado e finalizar o registro? Digite 'e'.");
        }
        printf("\nEste foi o ultimo aluno? Digite qualquer outra tecla.");
        fflush(stdin);
        scanf("%c", &opt);
        opt = tolower(opt);

        //Se digitou 'v', reduz um das variáveis de contagem.
        if(opt=='v'){
            i--;
            alu--;
        }

        //Adiciona um às variáveis de contagem.
        i++;
        alu++;

        //Desativa a opção excluir caso seja o primeiro aluno. Se não for e digitou 'e', entra no laço.
        if(i>0 && opt=='e'){
            i--;
            alu--;
            //Exclui tudo que possa ter sido digitado em "i"
            for(j=0;j<strlen(alunos[i]);j++){
            alunos[i][j]=' ';
            }
            prova[i][0]=' ';
            vprova[i]=' ';
            for(j=0;j<strlen(monal[i]);j++)
            monal[i][j]=' ';
        }

        system("cls");//Tela limpa

    }while(opt=='s'||opt=='v');

    //Alocando cada monitor a seus monitorados
    for(i=0; i<mon; i++){
            for(j=0; j<alu; j++){
                if(strcmp(monal[j], monitores[i])==0){
                        //Transfere para a nova string na ordem correta
                        while(alunos[j][y]!= '\0'){
                            monal2[x][y] = alunos[j][y];
                            y++;
                }

                //Zerando variável para reciclagem
                y=0;

                //Adiciona 1 à variável(para avançar a linha)
                x++;

                //Adiciona 1 à contagem de monitorados do monitor "i".
                contmon[i]++;
                }
            }
    }

    //Zerando variavel para reciclagem
    x=0;

    //Coloca a primeira letra do nome dos monitores como maiúscula (apenas por estética)
    for(i=0; i<mon; i++){
        monitores[i][0] = toupper(monitores[i][0]);
    }

    //Printf's para conferencia do usuário
    for(i=0; i<mon; i++){
            printf("----------------------------------------------------------------------------\n");
            printf("Alunos do monitor %s:\n", monitores[i]);
        for(j=0; j<contmon[i]; j++){
            printf("\n%s\n", monal2[j+k]);
        }
        k += contmon[i];
    }

    //Alocando os alunos para os vetores de cada prova
    for(i=0; i<alu; i++){
        if(vprova[i] == 'a'){
            strcpy(A[a], alunos[i]);
            fez[0][a] = i;//Guarda quais alunos (em numero de indice) fizeram a prova A
            n[0]++;
            a++;
        }else if(vprova[i] == 'b'){
            strcpy(B[b], alunos[i]);
            fez[1][b] = i;//Guarda quais alunos (em numero de indice) fizeram a prova B
            n[1]++;
            b++;
        }else{
            strcpy(C[c], alunos[i]);
            fez[2][c] = i;//Guarda quais alunos (em numero de indice) fizeram a prova C
            n[2]++;
            c++;
        }
    }

    //Calcula quantos monitores irao corrigir cada prova.
    variancia(a, b, c, alu, mon, mona, monb, monc, monabc);

    //Slots de espera de monitores (ficam vazios à espera de um monitor que se "encaixe" em suas condicoes, que sera o próximo passo)
    for(i=0; i<3; i++){
        int soma;

        soma=0;
        x=0;

        //Testa de qual prova estamos falando
        if(i==0)
            med[i] = a/(double)monabc[i];
        else if(i==1)
            med[i] = b/(double)monabc[i];
        else
            med[i] = c/(double)monabc[i];

        for(j=0; j<monabc[i]; j++){
                slot[i][j]= floor(med[i]);
                soma += slot[i][j];
        }
        //Fica somando um até que o valor, que foi arredondado pra baixo, se torne igual ao inteiro que representa o numero de monitores corretores
        while(soma<n[i]){
                slot[i][x]++;
                soma++;
                x++;
        }
    }

    /*Proximo passo (ainda nao realizado) será testar, com dois laços for aninhados se o monitor[i] tem slot[i][j] não-alunos
    para corrigir na prova 0<=i<3(A, B ou C).Este teste será executado em forma de permutacao em duas dimensoes (i e j) e aceitará,
    na ordem determinada, a primeira combinacao monitor-slot-prova que der certo*/

    for(i=0; i<3; i++){
        for(j=0; j<monabc[i]; j++){
            for(k=0;k<mon;k++){
                if(n[i]- monal3[k][i]>=slot[i][j] && mflag[k]==0){
                    mflag[k]++;
                    y=0;
                    for(x=0; x<n[i]; x++){
                        if(strcmp(monal[fez[i][x]],monitores[k])!=0 && y<slot[i][j] && aflag[fez[i][x]]==0){
                            strcpy(qcq[k][y], alunos[fez[i][x]]);
                            aflag[fez[i][x]]++;
                            y++;
                        }
                    }
                    while(y<slot[i][j]){
                        for(z=0; z<n[i]; z++){
                            t=0;
                            while(aflag[fez[i][z]]==0){
                                if(aflag[fez[i][t]]==1 && strcmp(monal[fez[i][t]], monitores[k])!=0){
                                strcpy(qcq[k][y], qcq[k][t]);
                                strcpy(qcq[k][t], alunos[fez[i][t]]);
                                aflag[fez[i][z]]++;
                                }
                                t++;
                            }
                        }
                        y++;
                    }
                }
            }
        }
    }

    i=0;
    j=0;

    for(i=0; i<mon; i++){
            printf("\nMonitor(a) %s corrige:\n", monitores[i]);
        for(j=0; j<contmon[i]; j++){
            printf("-%d: %s\n", j+1, qcq[i][j]);
        }
    }

    //Printfs apenas para teste do programa
    printf("\n\nO numero de monitores a corrigir a prova A eh: %d", monabc[0]);
    printf("\n\nO numero de monitores a corrigir a prova B eh: %d", monabc[1]);
    printf("\n\nO numero de monitores a corrigir a prova C eh: %d", monabc[2]);

    //Fim do programa
    printf("\n\n%cLucas Fischer e Marcelo Welzel\nMonitoria 22/1 Aprendizagem Autonoma Prof Alberto do Canto\nVer. 1.0 Alpha Pre-Release\nPressione qualquer tecla para finalizar.", 169);
    scanf("%d", &opt);

    return 0;
}
