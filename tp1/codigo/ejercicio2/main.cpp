#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Bishops.cpp : Defines the entry point for the console application.
//
using namespace std;

typedef long entero;

void go(vector<bool>& DiagonalSubida, vector<bool>& DiagonalBajada, int &k, int &n, entero& cantSoluciones, int cantAlfilesColocados, int ultimaFila, int ultimaColumna){

	int i,j;

	if (cantAlfilesColocados == k){
		cantSoluciones++;
	}else{

		j = ultimaColumna;
		for (i=ultimaFila; i<n; i++){
			while (j<n){

				if (!DiagonalSubida[i+j] && !DiagonalBajada[i-j+n-1]){

					DiagonalSubida[i+j] = true;
					DiagonalBajada[i-j+n-1] = true;

					go(DiagonalSubida, DiagonalBajada,k,n,cantSoluciones,cantAlfilesColocados+1, i, j+1);

					DiagonalSubida[i+j] = false;
					DiagonalBajada[i-j+n-1] = false;

				}
				j++;
			}
			j = 0;
		}
	}
}

entero cantSoluciones(int n, int k){

	if (k > n*n){
		return 0;
	}else{


		vector<bool> DiagonalSubida;
		vector<bool> DiagonalBajada;

		DiagonalBajada.resize((2*n)-1);
		DiagonalSubida.resize((2*n)-1);

		int i;

		for (i = 0; i<n; i++){
			DiagonalSubida[i] = false;
			DiagonalBajada[i] = false;
		}

		entero cantSoluciones = 0;
		int cantAlfilesColocados = 0;

		go(DiagonalSubida,DiagonalBajada,k,n,cantSoluciones,cantAlfilesColocados, 0, 0);

		return cantSoluciones;
	}
}

int main(int argc, char* argv[])
{
    /*int n, k;
    scanf("%i %i", &n, &k);

    entero cant;

    while (n != 0 || k != 0){
        cant = cantSoluciones(n,k);
        cout << cant << endl;
        scanf("%i %i", &n, &k);
    }*/

    cout << cantSoluciones(8,6) << endl;

	return 0;
}
