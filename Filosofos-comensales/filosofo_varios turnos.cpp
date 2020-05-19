#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

const int N = 5;                                                                                    // número de filósofos
int PENSANDO = 0;                                                                                   // el filósofo está pensando
int HAMBRIENTO = 1;                                                                                 // el filósofo tiene hambre y quedrá agarrar los tenedores
int COMIENDO = 2;                                                                                   // el filosofo está comiendo
#define IZQUIERDO (i+N-1)%N                                                                         // número del vecino izquierdo de i
#define DERECHO (i+1)%N                                                                             // número del vecino derecho de i
int estado[N];                                                                                      // arreglo que lleva registro del estado de todos
mutex s[N];                                                                                         // un semáforo por filósofo

void tomar_tenedores(int i);                                                                        //definiendo funcion tomar_tenedores
void devolver_tenedores(int i);                                                                     //definiendo funcion devolver tenedores
void probar(int i);                                                                                 //definiendo funcion probar esta funcion probará si podemos tomar los tenedores
void pensar(){PENSANDO = 1;}                                                                        //pone al filosofo a pensar
void comer(int i)
{
	cout<<"El filosofo "<<i<<" esta comiendo..."<<endl;                                             //comer
}
void filosofo(int i)                                                                                // i es el id de filosofo
{
	while(true){                                                                                    // el filosofo vive pensando y comiendo (se repite en forma indefinida)
		pensar();                                                                                   // el filósofo está pensando
		tomar_tenedores(i);                                                                         // adquiere dos tenedores o se bloquea
		comer(i);                                                                                   // comiendo
		devolver_tenedores(i);                                                                      // poniendo de vuelta ambos tenedores en la mesa
	}
}
void tomar_tenedores(int i)                                                                         // adquiere dos tenedores o se bloquea
{
    cout<<"El filosofo "<<i<<" tomando tenedores"<<endl;
    mutex m;
    m.lock();                                                                                       //para poder probar si puedo tomar tenedores primero debo hacer un lock
	estado[i] = HAMBRIENTO;                                                                         // el filósofo i tiene hambre
	probar(i);                                                                                      // intenta agarrar los dos tenedores
	m.unlock();                                                                                     //quitamos el lock y podemos seguir
	cout<<"El filosofo "<<i<<" Esperando que los tenedores se desocupen"<<endl;
	m.lock();                                                                                       //se bloquea si no se adquirieron los tenedores
}
void devolver_tenedores(int i)                                                                      //devolvemos los tenedores
{
    cout<<"El filosofo "<<i<<" devolviendo tenedores"<<endl;
    mutex m;
    m.lock();                                                                                       // hacemos lock para poder trabajar con los tenedores
	estado[i] = PENSANDO;                                                                           // el filósofo empieza a pensar porque termino de comer
	probar(IZQUIERDO);                                                                              // verifico si mi vecino izquierdo puede comer
	probar(DERECHO);                                                                                // verifico si mi vecino derecho puede comer
	m.unlock();                                                                                     // quitamos el lock y podemos seguir
}
void probar(int i)                                                                                  //intentar agarrar los tenedores
{
    cout<<"El filosofo "<<i<<" comprobando que los tenedores esten libres"<<endl;
	if (estado[i] == HAMBRIENTO && estado[IZQUIERDO] != COMIENDO && estado[DERECHO] != COMIENDO) {  //comprobando que los tenedores esten libres
		estado[i] = COMIENDO;                                                                       //comiendo
		s[i].unlock();                                                                              // quitamos el lock y podemos seguir (aquí trabajamos con el array)
	}
}
int main()
{
	vector<thread> filosofos_v;                                                                     //vector de filosofos para que con pocos cambios podamos modificar el numero de filosofos
	for(int i = 0; i < 5; i++)                                                                      // iterar por cada filosofo
	{
        filosofos_v.push_back(thread( filosofo,i));                                                 //creando hilo de filoso y aumentando al vector
    }
    for(int i = 0; i < 5; i++)                                                                      //Creando los join
    {
        filosofos_v[i].join();                                                                      //tenemos que esperar que terminen los demás hilos
    }
	return 0;
}
