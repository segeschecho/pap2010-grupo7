// Bishops.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>

using namespace std;

typedef vector<vector<int> > tTablero;

//const ocupado = true;
const desocupado = 0;

void mostrarMatriz(vector<vector<int> > t){
	int i,j;
	int n = t.size();

	for (i = 0; i<n; i++){
		for (j = 0; j<n; j++){
			cout << t[i][j] << " ";			
		}
		cout << endl;
	}
	cout << endl;
}


bool esAtacado(tTablero& t, int i, int j){
	int fila,columna;
	int n = t.size();

	fila = i+1;
	columna = j+1;
	while (fila < n && columna < n){
		if (t[fila][columna] != desocupado)
			return true;
		fila++;
		columna++;
	}

	fila = i+1;
	columna = j-1;
	while (fila < n && columna >= 0){
		if (t[fila][columna] != desocupado)
			return true;
		fila++;
		columna--;
	}

	fila = i-1;
	columna = j-1;
	while (fila >= 0 && columna >= 0){
		if (t[fila][columna] != desocupado)
			return true;
		fila--;
		columna--;
	}

	fila = i-1;
	columna = j+1;
	while (fila >= 0 && columna < n){
		if (t[fila][columna] != desocupado)
			return true;
		fila--;
		columna++;
	}

	return false;
}

bool seRespetanPosicionesRelativas(tTablero& t, int i, int j, int alfil){
	
	int n = t.size();

	int fila, columna;	

	for (fila = 0; fila<n; fila++){
		for (columna = 0; columna < n; columna++){
			
			if (fila < i){
				if (t[fila][columna] != desocupado && t[fila][columna] > alfil)
					return false;
			}else{

				if (fila == i){
					if (columna < j){
						if (t[fila][columna] != desocupado &&  t[fila][columna] > alfil)
							return false;
					}else{

						if (columna > j){
							if (t[fila][columna] != desocupado &&  t[fila][columna] < alfil)
								return false;
						}
					}

				}else{
					if (t[fila][columna] != desocupado && t[fila][columna] < alfil)
						return false;
				}
			}
			
		}		
	}

	return true;
}

void go(tTablero& t, int &k, int &n, int& cantSoluciones, int cantAlfilesColocados, int ultimoAlfilPuesto){

	int i,j;

	if (cantAlfilesColocados == k){
		cantSoluciones++;		
	}else{
		
		for (i=0; i<n; i++){
			for (j=0; j<n; j++){
				
				if (t[i][j] == desocupado && !esAtacado(t,i,j) && seRespetanPosicionesRelativas(t,i,j,ultimoAlfilPuesto + 1)){
					t[i][j] = ultimoAlfilPuesto + 1;
					go(t,k,n,cantSoluciones,cantAlfilesColocados+1, ultimoAlfilPuesto + 1);
					t[i][j] = desocupado;
				}				
			}
		}
	}	
}

int main(int argc, char* argv[])
{
	int k, n;
	tTablero tablero;
	int i, j;

	n = 8;
	k = 6;

	//Cargo la matriz con todos false
	tablero.resize(n);

	for (i = 0; i<n; i++){
		tablero[i].resize(n);
	}

	for (i = 0; i<n; i++){
		for (j = 0; j<n; j++){
			tablero[i][j] = false;
		}
	}
	/////////////////////////////////

	int cantSoluciones = 0;
	int cantAlfilesColocados = 0;
	int ultimoAlfilPuesto = 0;

	go(tablero,k,n,cantSoluciones,cantAlfilesColocados, ultimoAlfilPuesto);

	cout << "formas de poner alfiles: " << cantSoluciones << endl;

	return 0;
}