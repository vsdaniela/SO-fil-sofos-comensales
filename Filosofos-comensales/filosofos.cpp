#include <iostream>
#include <pthread.h>
#include <vector>
#include<stdio.h>
#include <unistd.h>
#include <ctime>
using namespace std;
int lleno=1;
int n=5; // n= 5 comensales
vector<bool> tenedores(n);//un vector de estados de los tenedores que nos indicará si ocupados o desocupados
#define IZQUIERDA(x)	((x) !=0 ? (x)-1 : (n-1)) //Nos retorna asiento en el que se encuentra el comensal de la izquierda de x
#define DERECHA(x)		(((x)+1)%n) //Nos dice el asiento en el que se encuentra el comensal de la derecha de x
void comer(vector<bool>& tenedores, int id_asiento, int estomago)
{
            if(tenedores[IZQUIERDA(id_asiento)] == 0 && tenedores[DERECHA(id_asiento)]==0){ //evaluamos si el tenedor izquiero y derecho se encuentran disponibles
                    //en caso están disponibles pasa a comer
                    cout<<"Filosofo id: "<<id_asiento<<" tomó los dos tenedores "<<IZQUIERDA(id_asiento)<<" - "<<DERECHA(id_asiento)<<endl;
                    tenedores[IZQUIERDA(id_asiento)]=tenedores[DERECHA(id_asiento)]=1;
                    estomago++;
                    cout<<"estoy comiendo..., mi estomago tiene"<<estomago<<endl;
                    tenedores[IZQUIERDA(id_asiento)]=tenedores[DERECHA(id_asiento)]=0;
            }
            else{//en caso estén ocupados esperará por estos
                cout<<"Estoy esperando los tenedores..."<<endl;
                usleep(0.01);
            }
}
void pensar(int estomago, int id_asiento)
{
            estomago--;//este estomago-- nos representa que mientras se está pensando, se tendrá más hambre
            cout<<"Filosofo id: "<<id_asiento<<", estoy pensando..."<<endl;
}
void *filosofo(void*)
{
    int estomago=0;
    int id_asiento=asiento;
    asiento++;
    while(true){
        comer(tenedores,id_asiento,estomago);
        double inicio=clock();
        pensar(estomago, id_asiento);
        double fin=clock();
        double total=(double(fin-inicio)/CLOCKS_PER_SEC);
        cout<<total<<endl;
        if(total>=0.02)//si pienda más de 0.02 segundos significa que estuvo esperando mucho por comer y morirá de hambre
            cout<<id_asiento<<"Murió de hambre esperando "<<total<<endl;
    }
}

int main()
{

    pthread_t f1,f2,f3,f4,f5;
	int check1 = pthread_create(&f1, NULL, &filosofo,NULL);
	int check2 = pthread_create(&f2, NULL, &filosofo,NULL);
	int check3 = pthread_create(&f3, NULL, &filosofo,NULL);
	int check4 = pthread_create(&f4, NULL, &filosofo,NULL);
	int check5 = pthread_create(&f5, NULL, &filosofo,NULL);
    if(!check1){
		pthread_join(f1, NULL);
	}
    if(!check2){
		pthread_join(f2, NULL);
	}
    if(!check3){
		pthread_join(f3, NULL);
	}
    if(!check4){
		pthread_join(f4, NULL);
	}
    if(!check5){
		pthread_join(f5, NULL);
	}
    return 0;
}
