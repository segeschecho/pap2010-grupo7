#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Bishops.cpp : Defines the entry point for the console application.
//
using namespace std;

typedef vector<vector<int> > tTablero;

struct tAlfil{
    bool enTablero;
    int fila;
    int columna;
};

const bool desocupado = 0;
const bool ocupado = 1;

bool esAtacado(vector<tAlfil>& alfiles, int i, int j){

	int cantAlfiles = alfiles.size();
    int indice;

    int k;
    int l;

    for (indice=0; indice<cantAlfiles; indice++){

        if (alfiles[indice].enTablero){

            k = alfiles[indice].fila;
            l = alfiles[indice].columna;

            if (i-j == k-l || i+j == k+l || j-l == i-k || j-l == k-i){
                return true;
            }
        }
    }

	return false;
}

//devuelve true si la primera posicion es menor a la segunda posicion en una matriz
bool menor(int fila1, int columna1, int fila2, int columna2){

    if (fila1 < fila2){
        return true;
    }
    else{
        if (fila1 > fila2){
            return false;
        }
        else{
            if (columna1 < columna2)
                return true;
            else
                return false;
        }
    }
}

bool seRespetanPosicionesRelativas(vector<tAlfil>& alfiles, int i, int j, int alfil){

	int cantAlfiles = alfiles.size();
    int k;

    for (k=0; k<alfil; k++){
        //PosicionalfilIndice > posicionAlfil
        if (alfiles[k].enTablero && !(menor(alfiles[k].fila, alfiles[k].columna, i, j))){
            return false;
        }
    }

    for (k=alfil+1; k<cantAlfiles; k++){
        //PosicionalfilIndice < posicionAlfil
        if (alfiles[k].enTablero && menor(alfiles[k].fila, alfiles[k].columna, i, j)){
            return false;
        }
    }

    return true;
}


void go(tTablero& t, vector<tAlfil>& alfiles, int &k, int &n, long& cantSoluciones, int cantAlfilesColocados, int ultimoAlfilPuesto, int ultimaFila){
//void go(tTablero& t, vector<tAlfil>& alfiles, int &k, int &n, long& cantSoluciones, int cantAlfilesColocados, int ultimoAlfilPuesto){

	int i,j;

	if (cantAlfilesColocados == k){
		cantSoluciones++;
	}else{

		//for (i=ultimaFila; i<n; i++){
		for (i=0; i<n; i++){
			for (j=0; j<n; j++){

				if (t[i][j] == desocupado && (!esAtacado(alfiles,i,j)) && seRespetanPosicionesRelativas(alfiles,i,j,ultimoAlfilPuesto)){

					t[i][j] = ultimoAlfilPuesto;
					alfiles[ultimoAlfilPuesto].enTablero = true;
					alfiles[ultimoAlfilPuesto].fila = i;
					alfiles[ultimoAlfilPuesto].columna = j;

					go(t,alfiles,k,n,cantSoluciones,cantAlfilesColocados+1,ultimoAlfilPuesto+1, i);
					//go(t,alfiles,k,n,cantSoluciones,cantAlfilesColocados+1,ultimoAlfilPuesto+1);

					t[i][j] = desocupado;
					alfiles[ultimoAlfilPuesto].enTablero = false;
					alfiles[ultimoAlfilPuesto].fila = -1;
					alfiles[ultimoAlfilPuesto].columna = -1;

				}
			}
		}
	}
}

long cantSoluciones(int n, int k){

	if (k > n*n){
		return 0;
	}else{

	    tTablero tablero;
		int i, j;

		//Cargo la matriz con todos false
		tablero.resize(n);

		for (i = 0; i<n; i++){
			tablero[i].resize(n);
		}

		for (i = 0; i<n; i++){
			for (j = 0; j<n; j++){
				tablero[i][j] = desocupado;
			}
		}

		vector<tAlfil> alfiles;

		//Cargo la matriz con todos false
		alfiles.resize(n);

		for (i=0; i<n; i++){
		    alfiles[i].fila = -1;
		    alfiles[i].columna = -1;
		    alfiles[i].enTablero = false;
		}

		long cantSoluciones = 0;
		int cantAlfilesColocados = 0;
		int ultimoAlfilPuesto = 0;

		go(tablero,alfiles,k,n,cantSoluciones,cantAlfilesColocados, ultimoAlfilPuesto, 0);

		return cantSoluciones;
	}
}

int main(int argc, char* argv[])
{
    int n, k;
    scanf("%i %i", &n, &k);

    long cant;

    while (n != 0 || k != 0){
        cant = cantSoluciones(n,k);
        cout << cant << endl;
        scanf("%i %i", &n, &k);
    }

	return 0;
}
