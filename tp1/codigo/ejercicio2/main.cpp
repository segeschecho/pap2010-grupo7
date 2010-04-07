/*#include <iostream>
#include <vector>

// Bishops.cpp : Defines the entry point for the console application.
//
using namespace std;

void go(vector<bool>& DiagonalSubida, vector<bool>& DiagonalBajada, int &k, int &n, long& cantSoluciones, int cantAlfilesColocados, int ultimaFila, int ultimaColumna){

	int i,j;

	if (cantAlfilesColocados == k){
		cantSoluciones++;
	}else{

		j = ultimaColumna;
		for (i=ultimaFila; i<n; i++){
			while (j<n){

				if (!DiagonalSubida[i+j] && !DiagonalBajada[i-j+n-1]){

                    //Pongo un elemento en la posicion i j
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

long cantSoluciones(int n, int k){

    vector<bool> DiagonalSubida;
    vector<bool> DiagonalBajada;

    DiagonalBajada.resize((2*n)-1);
    DiagonalSubida.resize((2*n)-1);

    int i;

    for (i = 0; i<n; i++){
        DiagonalSubida[i] = false;
        DiagonalBajada[i] = false;
    }

    long cantSoluciones = 0;
    int cantAlfilesColocados = 0;

    go(DiagonalSubida,DiagonalBajada,k,n,cantSoluciones,cantAlfilesColocados, 0, 0);

    return cantSoluciones;

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
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

// Bishops.cpp : Defines the entry point for the console application.
//
using namespace std;


bool par(int n){

	if (n%2 == 0)
		return true;

	return false;
}


//IMPORTANTE: Si se llama a esta funcion con contarNegras=true, el valor inicial de ultimaColumna tiene que ser 0, si se llama con contarNegras=false el valor inicial de ultimaColumna tiene que ser 1.
void go(vector<bool>& DiagonalSubida, vector<bool>& DiagonalBajada, int &k, int &n, long& cantSoluciones, int cantAlfilesColocados, int ultimaFila, int ultimaColumna, bool contarNegras){

	int i,j;

	if (cantAlfilesColocados == k){
		cantSoluciones++;
		//mostrarMatriz(DiagonalSubida,DiagonalBajada,n);
	}else{


		j = ultimaColumna;

		for (i=ultimaFila; i<n; i++){
			while (j<n){

				if (!DiagonalSubida[i+j] && !DiagonalBajada[i-j+n-1]){

                    //Pongo un elemento en la posicion i j
                    DiagonalSubida[i+j] = true;
                    DiagonalBajada[i-j+n-1] = true;

                    go(DiagonalSubida, DiagonalBajada,k,n,cantSoluciones,cantAlfilesColocados+1, i, j+2, contarNegras);

                    DiagonalSubida[i+j] = false;
                    DiagonalBajada[i-j+n-1] = false;

				}
				j+=2;
			}
			if (contarNegras){
				if (par(i+1))
					j = 0;
				else
					j = 1;
			}
			else{
				if (par(i+1))
					j = 1;
				else
					j = 0;
			}
		}
	}
}

long cantSoluciones(int n, int k){

  	if (k > 2*(n-1) && n!=1){
  	    return 0;
  	}else{
  	    if (k == 1){
  	        return (n*n);
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

            long cantSoluciones = 0;
            int cantAlfilesColocados = 0;

            if (par(n)){

                vector<long> solucionParcial;
                solucionParcial.resize(k+1);

                for (i=0; i<=k; i++){
                    cantSoluciones = 0;
                    go(DiagonalSubida,DiagonalBajada,i,n,cantSoluciones,cantAlfilesColocados, 0, 0, true);
                    solucionParcial[i] = cantSoluciones;
                }

                cantSoluciones = 0;

                for (i=0; i<=k; i++)
                    cantSoluciones = cantSoluciones + (solucionParcial[i] * solucionParcial[k-i]);

            }else{

                vector<long> solucionParcialBlancas;
                solucionParcialBlancas.resize(k+1);
                vector<long> solucionParcialNegras;
                solucionParcialNegras.resize(k+1);

                for (i=0; i<=k; i++){
                    cantSoluciones = 0;
                    go(DiagonalSubida,DiagonalBajada,i,n,cantSoluciones,cantAlfilesColocados, 0, 0, true);
                    solucionParcialNegras[i] = cantSoluciones;

                    cantSoluciones = 0;
                    go(DiagonalSubida,DiagonalBajada,i,n,cantSoluciones,cantAlfilesColocados, 0, 1, false);
                    solucionParcialBlancas[i] = cantSoluciones;
                }

                cantSoluciones = 0;

                for (i=0; i<=k; i++)
                    cantSoluciones = cantSoluciones + (solucionParcialNegras[i] * solucionParcialBlancas[k-i]);
            }

            return cantSoluciones;
        }
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


