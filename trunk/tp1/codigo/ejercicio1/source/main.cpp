#include <iostream>
#include <vector>
#include <cstdlib> //para abs
//#include "../../include/InputFile.h"

using namespace std;

int maxEditStepLadders(int fin);
vector <string> generarPalabras(int indice);


/* var global donde estan las palabras */
vector <string> palabras;
vector <int> vectorLargos;
int maximaEscalera;

int main()
{
/*    string s;
    int res = 1;
    //limpio el vector
    vectorLargos.clear();
    maximaEscalera = 1;

    while (cin >> s){
        //alargo el vector de las soluciones con valores 1
        vectorLargos.push_back(1);
        palabras.push_back(s);

        res = maxEditStepLadders(palabras.size());
    }

    cout << res;
*/

    string s;
    vector <string> vec;


    s="holacaca";
    palabras.push_back("hola");

    vec = generarPalabras(0);


    cout << "palabras generadas: " << endl;
    for (int i = 0; i < vec.size(); i++){
        cout << vec[i] << endl;
    }

    return 0;
}

int maxEditStepLadders(int fin){

    /***
    pasos:
    1: tomar las palabra y generar sus posibles edit ladders
    2: por cada una de estas palabras, ver si esta antes, usando busqueda
    binaria (por que las palabras estan ordenadas!!), se pueden saltear las
    palabras que sean mayores a la actual!!!
    3: me quedo con el mayor step ladder de las palabras que estan, y lo pongo
    en el valor de el elemento actual.
    4: actualizo la var global que tiene la MAXIMA step ladder.
    5: fin.
    ****/

    ///sacarr!!
    return 1;
}

vector <string> generarPalabras(int indice){
    /***
    a-z esta entre los ascii 97-122 inclusive
    ***/
    int largoPalabra = palabras[indice].size();
    char primeraLetra = palabras[indice][0];
    vector <string> res;

    /* saco de a una letra por vez y lo agrego a res */
    /* filtro tambien las palabras que son mayores a palabras[indice]*/
    ///saco y modifico letras
    for (int i = 0; i < largoPalabra; i++){
        string temp;
        string parteIzq;
        string parteDer;

        /* inicio las var */
        parteIzq = palabras[indice].substr(0, i);
        parteDer = palabras[indice].substr(i + 1, largoPalabra);

        /* saco la letra i */
        temp = parteIzq;
        temp.append(parteDer);

        /* la agrego a res */
        res.push_back(temp);

        ///modifico letras
        /// se pueden filtrar letras si i == 0
        for(int ascii = 97; ascii < 123; ascii++){
            temp.clear();
            temp = parteIzq;
            temp.push_back(ascii);
            temp.append(parteDer);

            /* la agrego a res */
            res.push_back(temp);
        }
    }


    ///agrego letras
    /* agrego letras al principio y filtro las que no van */
    for(int ascii = 97; ascii <= primeraLetra; ascii++){
        string temp;

        /* agrego la letra al principio */
        temp.push_back(ascii);
        temp.append(palabras[indice]);

        /* la agrego a res */
        res.push_back(temp);
        temp.clear();
    }

    /* agrego desde la segunda letra*/
    for(int i = 1; i <= largoPalabra; i++){
        /* agrego todas las posibles letras en la posicion i */
        for(int ascii = 97; ascii < 123; ascii++){
            string temp;
            string parteIzq;
            string parteDer;

            /* inicio las var */
            parteIzq = palabras[indice].substr(0, i);
            parteDer = palabras[indice].substr(i, largoPalabra);

            temp = parteIzq;
            temp.push_back(ascii);
            temp.append(parteDer);

            /* la agrego a res */
            res.push_back(temp);
            temp.clear();
        }

    }


    /* devuelvo todo */
    return res;
}
