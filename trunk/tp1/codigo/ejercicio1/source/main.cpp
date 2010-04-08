#include <iostream>
#include <vector>
#include <cstdlib> //para abs
//#include "../../include/InputFile.h"

using namespace std;

int letrasDiferentes(int p1, int p2);
bool sePuedeAgregar(int p1, int p2);
int maxEditStepLadders(int inicio, int fin);


/* var global donde estan las palabras */
vector <string> palabras;
vector <int> vectorLargos;
int maximaEscalera;

int main()
{
    string s;
    int res = 1;
    //limpio el vector
    vectorLargos.clear();
    maximaEscalera = 1;

    while (cin >> s){
        //alargo el vector de las soluciones con valores 1
        vectorLargos.push_back(1);
        palabras.push_back(s);

        res = maxEditStepLadders(0, palabras.size());
    }

    cout << res;

    return 0;
}

/***
* letrasDiferentes: toma dos indices de palabras en el arreglo palabras
* y devuelve la cantidad de letras diferentes entre estas,
* la cantidad de diferencias esta limitada a 2, ya que si hay 2 o mas diferencias
* ya no se puede obtener una de las palabras partiendo de la otra.
*
* La complejidad de esta funcion es lineal con respecto al largo de las palabras
* asociadas a los indices.
***/
int letrasDiferentes(int p1, int p2){
    string s1 = palabras[p1];
    string s2 = palabras[p2];

    int limite = min(s1.size(), s2.size());
    int diferencias = 0;
    int i = 0;

    cout << "comparando: " << s1 << " = " << s2 << " : " << s2.compare(s1) << endl;;

    /* recorro las palabras y veo que letras tienen diferentes */
    while (diferencias < 2 && i < limite){
        if(s1[i] != s2[i])
            diferencias++;
        i++;
    }

    return diferencias;
}


/***
* sePuedeAgregar: dice si la palabra se puede o no agregar deacuerdo a la restriccion
* que dice que las palabras tengan igual largo o difieran en a lo sumo 1 caracter y que
* tengan a lo sumo 1 caracter de diferencia.
*
* la complejidad es lineal en el largo de las palñabras asociadas a p1 y p2, ya que
* usa letrasDiferentes.
***/
bool sePuedeAgregar(int p1, int p2){

    int diferencia = abs( (int)(palabras[p1].size() - palabras[p2].size()) );

    if (diferencia > 1)
        return false;

    if (letrasDiferentes(p1, p2) >= 2){
        return false;
    }

    return true;
}

int maxEditStepLadders(int inicio, int fin){
    /* cantidad de elementos */
    int tam = fin - inicio;

    if (tam <= 2){
        if (tam == 1)
            return 1;
        else{
            vectorLargos[fin - 1] = sePuedeAgregar(0, fin - 1) + 1;
            return vectorLargos[fin - 1];
        }
    }

    /* recorro de atras para adelante */
//    for (int i = fin - 2; i >= inicio; i--){
        int mayorEscalera = 1; // siempre hay una escalera de largo 1
        /* me quedo con la escalera mas grande desde i hasta fin */
        for(int k = fin - 2; k >= 0; k--){

            if(sePuedeAgregar(k, fin - 1) && (1 + vectorLargos[k]) > mayorEscalera){
                mayorEscalera = 1 + vectorLargos[k];
            }
        }

        /* ahora sali del for y tengo la maxima escalera desde i a fin en maximaEscalera */
        vectorLargos[fin - 1] = mayorEscalera;
        if (mayorEscalera > maximaEscalera)
            maximaEscalera = mayorEscalera;
//    }

    /* ahora busco la escalera mas larga en el vector */
    return maximaEscalera;
}
