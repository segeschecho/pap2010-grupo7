#include <iostream>
#include <vector>
#include <cstdlib> //para abs
//#include "../../include/InputFile.h"

using namespace std;

int letrasDiferentes(int p1, int p2);
bool sePuedeAgregar(int p1, int p2);
int maxEditStepLadders(int palabraActual, int anteriorAgregada);


/* var global donde estan las palabras */
vector <string> palabras;
vector <int> vectorLargos;

int main()
{

    /*if (argc < 2){
        cout << "debe ingresar el nombre del archivo";
        return 1;
    }

    const char *archivo = argv[1];

*/
/*
    InputFile *a = new InputFile();
    //a->openFile(archivo);
    a->openFile("25milLargo5.txt");
    palabras = a->getFileContent();

    // genero el tamanio del vector y lo inicio en 1
    vectorLargos.clear();
    vectorLargos.resize(palabras.size(), 1);
*/
    string s;
    int i = 0;
    //limpio el vector
    vectorLargos.clear();

    cin >> s;

    while (s.compare("\\0") != 0){
        palabras.push_back(s);
        i++;
        //alargo el vector de las soluciones con valores 1
        vectorLargos.resize(i, 1);

        cin >> s;
    }

    cout << maxEditStepLadders(0, palabras.size() - 1);

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

/***
* recibe los indices BIEN!(el indice posteriors)
****/
int maxEditStepLadders(int inicio, int fin){
    /* cantidad de elementos */
    int tam = fin - inicio + 1;
    int maxRes = 1;

    if (tam <= 2){
        if (tam == 1)
            return 1;
        else
            return sePuedeAgregar(fin - 1, fin) + 1;
    }

    /* inicio los ultimos elementos de vectorLargos */
    /* si voy de la anteultima palabra a la ultima */
    /* entonces el el largo de la escalera desde fin-1 a fin es 2 */
    if(sePuedeAgregar(fin - 1, fin))
        vectorLargos[fin - 1] = 2;

    /* recorro de atras para adelante */
    for (int i = fin - 2; i >= inicio; i--){
        int maximaEscalera = 1; // siempre hay una escalera de largo 1
        /* me quedo con la escalera mas grande desde i hasta fin */
        for(int k = i + 1; k <= fin; k++){

            if(sePuedeAgregar(i, k) && (1 + vectorLargos[k]) > maximaEscalera){
                maximaEscalera = 1 + vectorLargos[k];
            }
        }

        /* ahora sali del for y tengo la maxima escalera desde i a fin en maximaEscalera */
        vectorLargos[i] = maximaEscalera;
        if (maximaEscalera > maxRes)
            maxRes = maximaEscalera;
    }

    /* ahora busco la escalera mas larga en el vector */
    return maxRes;
}
