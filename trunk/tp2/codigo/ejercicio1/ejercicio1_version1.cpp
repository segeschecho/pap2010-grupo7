//#define FILEINPUT
//#define TESTPERFORMANCE

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#define INFINITO 0xFFFFFFFF

enum Direccion{ ARRIBA, DERECHA, ABAJO, IZQUIERDA };

typedef unsigned int uint;
typedef pair< uint, uint > Casillero;

typedef pair< uint, uint > Puiui;

typedef vector< Puiui > VPuiui;
typedef vector< VPuiui > VvPuiui;
typedef vector< VvPuiui > VvvPuiui;
typedef vector< VvvPuiui > VvvvPuiui;

#ifdef TESTPERFORMANCE
#include "windows.h"
uint nodosAnalizados;
DWORD alocacionDeMatrices;
#endif

inline uint beta ( bool b ) { return b ? 1 : 0; }

class Nodo
{
public:
    Nodo(){};
    Nodo( Casillero posCaja, Casillero posKeeper ) : pC( posCaja ), pK( posKeeper ) {};

    void operator=( const Nodo& nodo ) { pC = nodo.pC; pK = nodo.pK; }

    Casillero pC;
    Casillero pK;
};

class TestCase
{
public:
    TestCase(){};
    ~TestCase(){};

    inline bool valido()
    {
        return mValido;
    }

    inline bool marcado( const Nodo& nodo )
    {
        return minDist( nodo ).first < INFINITO && minDist( nodo ).second < INFINITO;
    }

    inline Puiui& minDist( const Nodo& nodo )
    {
        return mMinDist[ nodo.pC.first ][ nodo.pC.second ][ nodo.pK.first ][ nodo.pK.second ];
    }

    inline bool hayPared( const Casillero& c )
    {
        return mHayPared[ c.first ][ c.second ];
    }

    inline bool esValido( const Nodo& nodo )
    {
        return !hayPared( nodo.pC ) && !hayPared( nodo.pK );
    }

    Puiui resolver()
    {
#ifdef TESTPERFORMANCE
        nodosAnalizados = 0;
        alocacionDeMatrices = 0;
#endif
        Puiui mejorSolucion = Puiui( INFINITO, INFINITO );

        uint filas = mHayPared.size();
        uint columnas = mHayPared[ 0 ].size();

#ifdef TESTPERFORMANCE
        DWORD inicio = GetTickCount();
#endif
        mMinDist = VvvvPuiui ( filas, VvvPuiui( columnas, VvPuiui( filas, VPuiui( columnas, Puiui(INFINITO, INFINITO) ) ) ) );
#ifdef TESTPERFORMANCE
        alocacionDeMatrices = GetTickCount() - inicio;
#endif
        // Comienzo del BFS
        vector< Nodo > colaCeros;
        vector< Nodo > colaUnos;

        colaCeros.push_back( Nodo(mCasInicialCaja, mCasInicial) );
        minDist( colaCeros.front() ) = Puiui( 0, 0 );

        uint indiceCeros = 0;
        uint indiceUnos = 0;

        while( indiceCeros < colaCeros.size() || indiceUnos < colaUnos.size() )
        {
            Nodo nodoActual;
            if( indiceCeros < colaCeros.size() )
            {
                nodoActual = colaCeros[ indiceCeros ];
                indiceCeros++;
            }
            else
            {
                nodoActual = colaUnos[ indiceUnos ];
                indiceUnos++;
            }


            Puiui& minDistActual = minDist( nodoActual );
            uint minPasosActual = minDistActual.first;
            uint minEmpujonesActual = minDistActual.second;

            // Actualizo la mejor solucion hasta el momento
            if( nodoActual.pC == mGoal && \
                ( minEmpujonesActual < mejorSolucion.second || ( minEmpujonesActual == mejorSolucion.second && minPasosActual < mejorSolucion.first ) ) )
            {
                mejorSolucion = minDistActual;
            }
            // Si se esta yendo por un camino peor que la mejor solucion hasta el momento
            // corto, porque se que no voy a encontrar mejor solucion
            else
            {
                for( Direccion d = ARRIBA; d <= IZQUIERDA; d = static_cast< Direccion >( d + 1 ) )
                {
                    Nodo nodoNuevo = adyacente( nodoActual, d );
                    if( esValido( nodoNuevo ) && !marcado( nodoNuevo ) )
                    {
#ifdef TESTPERFORMANCE
                        nodosAnalizados++;
#endif
                        bool empujon = nodoNuevo.pC == nodoActual.pC ? false : true;
                        empujon ? colaUnos.push_back( nodoNuevo ) : colaCeros.push_back( nodoNuevo );

                        Puiui& minDistNuevo = minDist( nodoNuevo );
                        uint minPasosNuevo = minDistNuevo.first;
                        uint minEmpujonesNuevo = minDistNuevo.second;

                        if( minEmpujonesActual + beta(empujon) < minEmpujonesNuevo || ( minEmpujonesActual + beta(empujon) == minEmpujonesNuevo && minPasosActual + 1 < minPasosNuevo ) )
                        {
                            minDist( nodoNuevo ) = Puiui( minPasosActual + 1, minEmpujonesActual + beta(empujon) );
                        }
                    }
                }
            }
        }
        // Fin del BFS

        return mejorSolucion;
    }

    Casillero casilleroVecino( const Casillero& c, const Direccion& d )
    {
        uint fila = c.first, columna = c.second;
        uint nuevaFila = fila, nuevaCol = columna;

        switch( d )
        {
        case IZQUIERDA:
            nuevaCol = columna - 1;
            break;

        case DERECHA:
            nuevaCol = columna + 1;
            break;

        case ARRIBA:
            nuevaFila = fila - 1;
            break;

        case ABAJO:
            nuevaFila = fila + 1;
            break;
        };

        return Casillero( nuevaFila, nuevaCol );
    }

    Nodo adyacente( const Nodo& n , const Direccion& d )
    {
        Casillero nuevoCasilleroK = casilleroVecino( n.pK, d ), nuevoCasilleroC = n.pC;
        nuevoCasilleroC = nuevoCasilleroK == n.pC ? casilleroVecino( n.pC, d ) : n.pC;

        return Nodo( nuevoCasilleroC, nuevoCasilleroK );
    }

    // casillero inicial de la caja
    Casillero mCasInicialCaja;

    // destino donde llegar la caja
    Casillero mGoal;

    // casillero inicial del cuidador del almacén (wharehouse keeper)
    Casillero mCasInicial;

    // representativo de las paredes del almacén
    vector< vector< bool > > mHayPared;

    // auxiliar para hacer el BFS
    VvvvPuiui mMinDist;

    bool mValido;
};

istream& operator>>( istream& is, TestCase& t )
{
    uint filas, columnas;
    if( is >> filas )
    {
        is >> columnas;
        if( filas == 0 && columnas == 0 )
        {
            t.mValido = false;
            return is;
        }

        // ignoro el caracter de '\n' que hay luego del k
#ifdef FILEINPUT
        is.ignore();
#else
        getchar();
#endif

        // FIXME: ARREGLAR ESTO EN EL MOMENTO DE LA ENTREGA PARA QUE SEA GETCHAR
#ifdef FILEINPUT
        char c = is.peek();
        is.ignore();
#else
        char c = getchar();
#endif
        // el filas + 2 y columnas + 2 es para agregarle una pared alrededor
        t.mHayPared = vector< vector< bool > > ( filas + 2, vector< bool >( columnas + 2, true ) );

        // parseamos filas x columnas caracteres
        for( uint fila = 1; fila < filas + 1; fila++ )
        {
            for( uint col = 1; col < columnas + 1; col++ )
            {
                if( c == '\n' )
                {
#ifdef FILEINPUT
                    c = is.peek();
                    is.ignore();
#else
                    c = getchar();
#endif
                }

                switch( c )
                {
                case '.':
                    t.mHayPared[ fila ][ col ] = false;
                    break;

                case 'T':
                    t.mGoal = Casillero( fila, col );
                    t.mHayPared[ fila ][ col ] = false;
                    break;

                case 'B':
                    t.mCasInicialCaja = Casillero( fila, col );
                    t.mHayPared[ fila ][ col ] = false;
                    break;

                case 'S':
                    t.mCasInicial = Casillero( fila, col );
                    t.mHayPared[ fila ][ col ] = false;
                    break;

                default:
                    break;
                };

#ifdef FILEINPUT
                c = is.peek();
                is.ignore();
#else
                c = getchar();
#endif
            }
        }
    }

    return is;
}

int main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
#else
    istream& entrada = cin;
#endif

    int numTest = 1;
    bool testValido = true;
    while( testValido )
    {
#ifdef TESTPERFORMANCE
        DWORD inicio = GetTickCount();
#endif

        TestCase testActual;
        entrada >> testActual;
        testValido = testActual.valido();
        if( testValido )
        {
            Puiui res = testActual.resolver();

            cout << "Instancia " << numTest << endl;
            if( res.first < INFINITO && res.second < INFINITO )
                cout << res.first << " " << res.second << endl;
            else
                cout << "Impossivel" << endl;
#ifdef TESTPERFORMANCE
            cout << GetTickCount() - inicio - alocacionDeMatrices << " ticks para la solucion" << endl;
            cout << alocacionDeMatrices << " ticks para alocacion de matrices" << endl;
            cout << nodosAnalizados << " nodos analizados" << endl;
#endif
            cout << endl;

            numTest++;
        }

    }
#ifdef FILEINPUT
    entrada.close();
    system("PAUSE");
#endif
}
