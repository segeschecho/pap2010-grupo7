#include <iostream>
#include <vector>
#include <cstdlib> //para abs
#include "../../include/InputFile.h"

using namespace std;

int letrasDiferentes(int p1, int p2);
bool sePuedeAgregar(int p1, int p2);
int maxEditStepLadders(int palabraActual, int anteriorAgregada);

/* var global donde estan las palabras */
vector <string> palabras;

int main()
{
    InputFile *a = new InputFile();
    a->openFile("pepe.txt");

    vector <string > v = a->getFileContent();

    cout << "palabra :" << v.at(0) << endl;

    palabras.push_back("cat");
    palabras.push_back("dig");
    palabras.push_back("dog");
    palabras.push_back("fig");
    palabras.push_back("fin");
    palabras.push_back("fine");
    palabras.push_back("fog");
    palabras.push_back("log");
    palabras.push_back("wine");


    cout << maxEditStepLadders(0, -1) << endl;

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


int maxEditStepLadders(int palabraActual, int anteriorAgregada){
    int resSi = 0; /* cuenta la cadena si agrego la palabra actual */
    int resNo = 0; /* cuenta la cadena si no la agrego */

    /* me fijo si llegue al final */
    if(palabraActual == palabras.size()){
        return 0;
    }

    /* me fijo si puedo no agregar la palabra */
    if ((anteriorAgregada == -1) || (anteriorAgregada >= 0 && sePuedeAgregar(palabraActual, anteriorAgregada))){

        resSi = 1 + maxEditStepLadders(palabraActual + 1, palabraActual);
    }

    /*me fijo cuantas cadenas obtengo si no agrego la palabra actual*/

    resNo = maxEditStepLadders(palabraActual + 1, anteriorAgregada);

    /* devuelvo el maximo de las cadenas */
    return max(resSi, resNo);
}
