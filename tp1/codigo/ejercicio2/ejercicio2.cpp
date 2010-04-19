#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

enum Color { Negro, Blanco };

inline bool par( int n )
{
    return n % 2 == 0;
}

// IMPORTANTE: Si se llama a esta funcion con color negro, el valor inicial de columna tiene que ser 1, sino 0.
long ubicarAlfilesEnColor( int n, int k, Color c, vector< bool >& DiagonalSubida, vector< bool >& DiagonalBajada, int filaInicial, int columnaInicial )
{
    // caso k == 0
    if( k == 0 )
    {
		return 1;
	}

    // caso k > 1
    long soluciones = 0;
	for( int fila = filaInicial; fila < n; fila++ )
    {
        for( int columna = columnaInicial; columna < n; columna += 2 )
        // voy sumandole de a 2 a j, para mantenerme en el mismo color
        {
            // Si esa diagonal esta libre, es decir, si ubicando el la fila y columna no se ataca con otro
			if ( !DiagonalSubida[ fila + columna ] && !DiagonalBajada[ fila - columna + n - 1 ] )
            {
                // Ubico un alfil en la posicion fila columna
                DiagonalSubida[ fila + columna ] = true;
                DiagonalBajada[ fila - columna + n - 1 ] = true;

                soluciones += ubicarAlfilesEnColor( n, k - 1, c, DiagonalSubida, DiagonalBajada, fila, columna + 2 );

                DiagonalSubida[ fila + columna ] = false;
                DiagonalBajada[ fila - columna + n - 1 ] = false;
			}
		}

        // esto es necesario porque en el tablero las filas pares empiezan en blanco
        // y las impares en negro
        if( ( par( fila + 1 ) && c == Negro ) || ( !par( fila + 1 ) && c == Blanco ) )
        {
            columnaInicial = 1;
        }
        else
        {
            columnaInicial = 0;
        }
	}

    return soluciones;
}

long solucionesUnColor( int n, int alfiles, Color color )
{
    vector< bool > DiagonalSubida( 2 * n - 1, false );
    vector< bool > DiagonalBajada( 2 * n - 1, false );

    // para nosotros, los tableros de ajedrez comienzan con el color blanco en el casillero 0,0
    // y en la proxima fila, los colores se invierten, es decir, si en una fila comienza el negro,
    // en la proxima comenzara el blanco, y viceversa
    if( color == Blanco )
    {
        return ubicarAlfilesEnColor( n, alfiles, color, DiagonalSubida, DiagonalBajada, 0, 0 );
    }
    else
    {
        return ubicarAlfilesEnColor( n, alfiles, color, DiagonalSubida, DiagonalBajada, 0, 1 );
    }
}

long cantSoluciones( int n, int k )
{
    // si k > 2 * ( n - 1 ) entonces la cantidad de formas en las que se pueden ubicar
    // k alfiles en un tablero de n*n es 0, pues en todo tablero de n*n hay
    // 2 * ( 2 * n - 1 ) diagonales y cada alfil ocupa dos diagonales, por lo tanto
    // luego de ubicar el alfil número 2 * ( n - 1 ) no hay forma de posicionar
    // el siguiente alfil sin que se ataque con otro.
  	if( k > 2 * ( n - 1 ) && n > 1 )
    {
  	    return 0;
  	}

    long cantSoluciones = 0;
    // si n es par, el tablero tiene igual cantidad de casilleros negros que blancos, por lo tanto puedo calcular
    // la cantidad de soluciones para un solo color, y estar seguro que sera la misma cantidad de soluciones que
    // para el otro color
    if( par( n ) )
    {
        vector< long > solucionParcial( k + 1, 0 );

        // pueden haber desde 0 hasta k alfiles en un solo color, por lo tanto vamos ubicando
        // 0, 1, ..., k alfiles para un solo color y luego hacemos la sumatoria
        for (int i = 0; i <= k && i < n; i++ )
        {
            solucionParcial[ i ] = solucionesUnColor( n, i, Negro );
        }

        for( int i = 0; i <= k; i++ )
        {
            cantSoluciones = cantSoluciones + ( solucionParcial[ i ] * solucionParcial[ k - i ] );
        }
    }
    // si n es impar, la cantidad de casilleros blancos difiere en uno con la cantidad de casilleros negros.
    else
    {
        vector< long > solucionParcialBlancas( k + 1, 0 );
        vector< long > solucionParcialNegras( k + 1, 0 );

        for ( int i = 0; i <= k && i <= n; i++ )
        {
            solucionParcialNegras[ i ] = solucionesUnColor( n, i, Negro );
            solucionParcialBlancas[ i ] = solucionesUnColor( n, i, Blanco );
        }

        for ( int i = 0; i <= k; i++ )
        {
            cantSoluciones = cantSoluciones + ( solucionParcialNegras[ i ] * solucionParcialBlancas[ k - i ] );
        }
    }

    return cantSoluciones;
}

int main(int argc, char* argv[])
{
    int n, k;
    scanf( "%i %i", &n, &k );

    while ( n != 0 || k != 0 ){
        cout << cantSoluciones( n, k ) << endl;
        scanf( "%i %i", &n, &k );
    }

	return 0;
}


