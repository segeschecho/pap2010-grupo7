//#define FILEINPUT

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

typedef unsigned int uint;
typedef pair< uint, uint > Casillero;

typedef pair< uint, uint > Puiui;

typedef vector< Puiui > Vpuiui;
typedef vector< Vpuiui > Vvpuiui;
typedef vector< Vvpuiui > Vvvpuiui;
typedef vector< Vvvpuiui > Vvvvpuiui;

#define INFINITO 0xFFFFFFFF

enum Direccion{ ARRIBA, DERECHA, ABAJO, IZQUIERDA };

class Nodo
{
public:
    Nodo( Casillero posCaja, Casillero posKeeper, uint pasos = 0, uint empujones = 0 ) : pC( posCaja ), pK( posKeeper ), p( pasos ), e( empujones ) {};
    Casillero pC;
    Casillero pK;
    uint p;
    uint e;
};

class TestCase
{
public:
    TestCase(){};
    ~TestCase(){};

    bool valido()
    {
        return mValido;
    }

    pair< int, int > resolver()
    {
        uint filas = hayPared.size();
        uint columnas = hayPared[ 0 ].size();

        Vvvvpuiui visite( filas, Vvvpuiui( columnas, Vvpuiui( filas, Vpuiui( columnas, Puiui(INFINITO, INFINITO) ) ) ) );
        vector< Nodo > cola;

        cola.push_back( Nodo(mCasInicialCaja, mCasInicial) );
        visite[ mCasInicialCaja.first ][ mCasInicialCaja.second ][ mCasInicial.first ][ mCasInicial.second ] = Puiui( 0, 0 );

        uint numNodoActual = 0;
        uint empujonesMejorSol = INFINITO;
        uint pasosMejorSol = INFINITO;

        // mientras que aun hayan acciones por realizar y la caja no esta en el destino
        while( numNodoActual < cola.size() )
        {
            Nodo nodoActual = cola[ numNodoActual ];

            Casillero pC = nodoActual.pC;
            Casillero pK = nodoActual.pK;

            if( pC == mGoal && ( nodoActual.e < empujonesMejorSol || ( nodoActual.e == empujonesMejorSol && nodoActual.p < pasosMejorSol ) ) )
            {
                empujonesMejorSol = nodoActual.e;
                pasosMejorSol = nodoActual.p;
            }
            else
            {
                for( Direccion d = ARRIBA; d <= IZQUIERDA; d = static_cast< Direccion >( d + 1 ) )
                {
                    Nodo nuevoNodo = adyacente( nodoActual, d );
                    uint menorCantPasos = visite[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ][ nuevoNodo.pK.first ][ nuevoNodo.pK.second ].first;
                    uint menorCantEmpujones = visite[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ][ nuevoNodo.pK.first ][ nuevoNodo.pK.second ].second;

                    if( !hayPared[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ] && !hayPared[ nuevoNodo.pK.first ][ nuevoNodo.pK.second ] && \
                        ( nuevoNodo.e < menorCantEmpujones || ( nuevoNodo.e == menorCantEmpujones && nuevoNodo.p < menorCantPasos ) ) && \
                        ( nuevoNodo.e < empujonesMejorSol || ( nuevoNodo.e == empujonesMejorSol && nuevoNodo.p < pasosMejorSol ) ) )
                    {
                        cola.push_back( nuevoNodo );
                        visite[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ][ nuevoNodo.pK.first ][ nuevoNodo.pK.second ] = Puiui( nuevoNodo.p, nuevoNodo.e );
                    }
                }
            }

            numNodoActual++;
        }

        if( pasosMejorSol == INFINITO && empujonesMejorSol == INFINITO )
        {
            return pair< int, int >( -1, -1 );
        }
        return pair< int, int >( pasosMejorSol, empujonesMejorSol );
    }
/*

    int moverme( Casillero ci, Casillero cf, Casillero casCaja )
    {
        if( hayPared[ cf.first ][ cf.second] )
        {
            return -1;
        }

        uint filas = hayPared.size();
        uint columnas = hayPared[ 0 ].size();

        Vvpuiui visite( Vvpuiui( filas, Vpuiui( columnas, false ) ) );
        vector< pair< Casillero, uint > > cola;

        cola.push_back( pair< Casillero, uint >( ci, 0 ) );
        visite[ cola[ 0 ].first.first ][ cola[ 0 ].first.second ] = true;
        uint i = 0;

        // mientras que aun me pueda mover y no estoy en el destino
        while( i < cola.size() && cola[ i ].first != cf )
        {
            for( Direccion d = ARRIBA; d <= IZQUIERDA; d = static_cast< Direccion >( d + 1 ) )
            {
                Casillero nuevoCasillero = casilleroVecino( cola[ i ].first, d );
                if( !hayPared[ nuevoCasillero.first ][ nuevoCasillero.second ] && nuevoCasillero != casCaja && 
                    !visite[ nuevoCasillero.first ][ nuevoCasillero.second ] )
                {
                    cola.push_back( pair< Casillero, uint >( nuevoCasillero, cola[ i ].second + 1 ) );
                    visite[ nuevoCasillero.first ][ nuevoCasillero.second ] = true;
                }
            }

            i++;
        }

        if( i == cola.size() )
        {
            return -1;
        }
        return cola[ i ].second;
    }
*/

    Casillero casilleroVecino( Casillero c, Direccion d )
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

    Nodo adyacente( Nodo n , Direccion d )
    {
        Casillero nuevoCasilleroK = casilleroVecino( n.pK, d ), nuevoCasilleroC = n.pC;
        uint empujo = 0;
        if ( nuevoCasilleroK == n.pC )
        {
            nuevoCasilleroC = casilleroVecino( n.pC, d );
            empujo = 1;
        }

        return Nodo( nuevoCasilleroC, nuevoCasilleroK, n.p + 1, n.e + empujo );
    }

    // casillero inicial de la caja
    Casillero mCasInicialCaja;

    // destino donde llegar la caja
    Casillero mGoal;

    // casillero inicial del cuidador del almacén (wharehouse keeper)
    Casillero mCasInicial;

    // representativo de las paredes del almacén
    vector< vector< bool > > hayPared;

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
        t.hayPared = vector< vector< bool > > ( filas + 2, vector< bool >( columnas + 2, true ) );

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
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'T':
                    t.mGoal = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'B':
                    t.mCasInicialCaja = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'S':
                    t.mCasInicial = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
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

    ifstream entrada( "testImpossivel", ios_base::in );
    ofstream salida( "testImpossivelOut", ios_base::out );
#else
    istream& entrada = cin;
    ostream& salida = cout;
#endif

    int numTest = 1;
    bool testValido = true;
    while( testValido )
    {
        TestCase testActual;
        entrada >> testActual;
        testValido = testActual.valido();
        if( testValido )
        {
            pair< int, int > res = testActual.resolver();

            salida << "Instancia " << numTest << endl;
            if( res.first >= 0 )
                salida << res.first << " " << res.second << endl;
            else
                salida << "Impossivel" << endl;

            salida << endl;

            numTest++;
        }
    }
#ifdef FILEINPUT
    salida.close();
    entrada.close();
#endif
}
