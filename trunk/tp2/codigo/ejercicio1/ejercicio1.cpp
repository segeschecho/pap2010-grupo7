#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

typedef unsigned int uint;
typedef uint Casillero;

inline int max( int a, int b )
{
    if( a > b )
        return a;
    return b;
}

//-----------------------------------------------------------------------------------
// Clase UInt
//-----------------------------------------------------------------------------------

/*
 *  Esta clase representa un entero sin signo que posiblemente puede ser infinito.
 *  Se provee de los métodos mínimos y necesarios para resolver el ejercicio.
 */
class UInt
{
public:
    // Constructores
    //-----------------------------------------------------------------------------------
    UInt() : mInfinite ( true ) {};
    UInt( uint valor ) : mInfinite( false ), mEntero( valor ){};

    // Operadores básicos
    //-----------------------------------------------------------------------------------
    void operator=( uint valor ) { mInfinite = false; mEntero = valor; }
    void operator=( const UInt& int2 ) { mInfinite = int2.mInfinite; mEntero = int2.mEntero; }
    bool operator>( const UInt& int2 ) const { return ( !mInfinite && !int2.mInfinite ) ? mEntero > int2.mEntero : mInfinite && !int2.mInfinite; }
    UInt operator+( uint valor ) const { return !mInfinite ? mEntero + valor : UInt(); }
    UInt operator+( UInt int2 ) const { return infinito() || int2.infinito() ? UInt() : UInt( mEntero + int2.mEntero ); }
    bool operator==( const UInt& int2 ) { return mInfinite == int2.mInfinite && mEntero == int2.mEntero; }

    // Observadores
    //-----------------------------------------------------------------------------------
    bool infinito() const { return mInfinite; }
    uint valor() const { return mEntero; }

private:
    // Atributos
    //-----------------------------------------------------------------------------------
    bool mInfinite;
    uint mEntero;
};

enum Direccion{ ARRIBA, IZQUIERDA, ABAJO, DERECHA };
Direccion dirContraria( Direccion d )
{
    return static_cast< Direccion > ( ( d + 2 ) % 4 );
}

class Almacen
{
public:

    class Vecino
    {
    public:
        Vecino( Casillero vec, Direccion d ) : vecino( vec ), dir( d ) {};

        Casillero vecino;
        Direccion dir;
    };

    Almacen(){};

    Almacen( int cantidadNodos ) : listaVecinos( cantidadNodos ){};
    ~Almacen(){};

    void agregarConeccion( Casillero cas1, Casillero cas2, Direccion d )
    {
        if( cas1 >= listaVecinos.size() || cas2 >= listaVecinos.size() )
        {
            listaVecinos.resize( max( cas1, cas2 ) + 1 );
        }

        listaVecinos[ cas1 ].push_back( Vecino( cas2, d ) );
        listaVecinos[ cas2 ].push_back( Vecino( cas1, dirContraria( d ) ) );
    }

    const vector< Vecino >& getVecinos( Casillero cas )
    {
        return listaVecinos[ cas ];
    }

    const Casillero* getCasVecino( Casillero cas, Direccion d )
    {
        for( vector< Vecino >::const_iterator itVec = listaVecinos[ cas ].begin(); itVec < listaVecinos[ cas ].end(); itVec++ )
        {
            if( itVec->dir == d )
            {
                return &itVec->vecino;
            }
        }

        return NULL;
    }

    Casillero getCantNodos()
    {
        return listaVecinos.size();
    }

    void agregarCasillero()
    {
        listaVecinos.resize( listaVecinos.size() + 1 );
    }

    void clear()
    {
        listaVecinos.clear();
    }

private:
    vector< vector< Vecino > > listaVecinos;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // primero limpio todas las variables miembro de t
        t.mFilas = 0;
        t.mColumnas = 0;
        t.mPosInicialCaja = -1;
        t.mGoal = -1;
        t.mPosInicial = -1;
        t.mAlmac.clear();

        // obtengo el k correspondiente al enunciado
        if( is >> t.mFilas )
        {
            is >> t.mColumnas;
            int ultimoNodo = 0;

            // ignoro el caracter de '\n' que hay luego del k
            is.ignore();
            // FIXME: ARREGLAR ESTO EN EL MOMENTO DE LA ENTREGA PARA QUE SEA GETCHAR
            char c = is.peek();
            is.ignore();

            // esta matriz representa los lugares donde hay pared
            vector< vector < bool > > hayPared ( t.mFilas, vector< bool >( t.mColumnas, false ) );

            // este es el mapa de casillero en la matriz a numero de cas
            map< int, Casillero > nodoDeCasillero;

            // parseamos filas x columnas caracteres
            for( int fila = 0; fila < t.mFilas; fila++ )
            {
                for( int col = 0; col < t.mColumnas; col++ )
                {
                    if( c == '\n' )
                    {
                        c = is.peek();
                        is.ignore();
                    }

                    if( c == '#' )
                    {
                        hayPared[ fila ][ col ] = true;
                    }
                    else
                    {
                        t.mAlmac.agregarCasillero();
                        nodoDeCasillero.insert( pair<int, Casillero>( fila*t.mColumnas + col, ultimoNodo ) );

                        // agregar un eje al cas de la izquierda, si es que existe y no hay pared
                        if( col > 0 && !hayPared[ fila ][ col - 1 ] )
                        {
                            t.mAlmac.agregarConeccion( ultimoNodo, ultimoNodo - 1, IZQUIERDA );
                        }

                        // agregar un eje al cas de arriba, si es que existe y no hay pared
                        if( fila > 0 && !hayPared[ fila - 1 ][ col ] )
                        {
                            t.mAlmac.agregarConeccion( ultimoNodo, nodoDeCasillero[ (fila - 1)*t.mColumnas + col ], ARRIBA );
                        }

                        if( c == 'T' )
                        {
                            t.mGoal = ultimoNodo;
                        }
                        else
                        {
                            if( c == 'B' )
                            {
                                t.mPosInicialCaja = ultimoNodo;
                            }
                            else
                            {
                                if( c == 'S' )
                                {
                                    t.mPosInicial = ultimoNodo;
                                }
                            }
                        }

                        ultimoNodo++;
                    }

                    c = is.peek();
                    is.ignore();
                }
            }
        }

        return is;
    }
public:
    TestCase(){};
    ~TestCase(){};

/*
 1  function Dijkstra(Graph, source):
 2      for each vertex v in Graph:           // Initializations
 3          dist[v] := infinity               // Unknown distance function from source to v
 4          previous[v] := undefined          // Previous node in optimal path from source
 5      dist[source] := 0                     // Distance from source to source
 6      Q := the set of all nodes in Graph
        // All nodes in the graph are unoptimized - thus are in Q
 7      while Q is not empty:                 // The main loop
 8          u := vertex in Q with smallest dist[]
 9          if dist[u] = infinity:
10              break                         // all remaining vertices are inaccessible from source
11          remove u from Q
12          for each neighbor v of u:         // where v has not yet been removed from Q.
13              alt := dist[u] + dist_between(u, v)
14              if alt < dist[v]:             // Relax (u,v,a)
15                  dist[v] := alt
16                  previous[v] := u
17      return dist[]
*/

    pair< int, int > resolver()
    {
        // movB y movS representan la mínima cantidad de empujones y de movimientos desde el origen
        // respectivamente. Se inicializan ambos en infinito.
        // posS representa la posición en la que terminé para llegar a la mínima cantidad de empujones y/o
        // de movimientos de ese cas.
        vector< UInt > movB( mAlmac.getCantNodos() );
        vector< UInt > movS( mAlmac.getCantNodos() );
        vector< Casillero > posS( mAlmac.getCantNodos() );

        int posCaja = mPosInicialCaja;
        movB[ posCaja ] = 0;
        posS[ posCaja ] = mPosInicial;

        vector< Casillero > nodosAVisitar;
        for( Casillero nodoAct = 0; nodoAct < mAlmac.getCantNodos(); nodoAct++ )
        {
            nodosAVisitar.push_back( nodoAct );
        }

        while( !nodosAVisitar.empty() )
        {
            Casillero nodoAct;
            UInt movBNodoAct;
            UInt movSNodoAct;

            vector< Casillero >::const_iterator nodoMenor;
            for( vector< Casillero >::const_iterator it = nodosAVisitar.begin(); it < nodosAVisitar.end(); it++ )
            {
                if(   movBNodoAct >  movB[ *it ] || \
                    ( movBNodoAct == movB[ *it ] && movSNodoAct > movS[ *it ] ) )
                {
                    nodoMenor = it;
                    movBNodoAct = movB[ *it ];
                    movSNodoAct = movS[ *it ];
                }
            }

            if( movBNodoAct.infinito() )
            {
                break;
            }

            nodoAct = *nodoMenor;
            nodosAVisitar.erase( nodoMenor );

            const vector< Almacen::Vecino >& vecinos = mAlmac.getVecinos( nodoAct );
            for( vector< Almacen::Vecino >::const_iterator itVec = vecinos.begin(); itVec < vecinos.end(); itVec++ )
            {
                int pasos = puedoMover( nodoAct, itVec->dir, posS[ nodoAct ] );
                if(  pasos >= 0 &&  \
                   ( movB[ itVec->vecino ] >  movBNodoAct + 1 || \
                   ( movB[ itVec->vecino ] == movBNodoAct && movS[ itVec->vecino ] > movSNodoAct + 1 ) ) )
                {
                    movB[ itVec->vecino ] = movBNodoAct + 1;
                    movS[ itVec->vecino ] = movSNodoAct.infinito() ? pasos : movSNodoAct + pasos;
                    posS[ itVec->vecino ] = nodoAct;
                }
            }
        }

        if( movB[ mGoal ].infinito() )
        {
            return pair< int, int >( -1, -1 );
        }

        return pair< int, int >( ( movB[ mGoal ] + movS[ mGoal ] ).valor(), movB[ mGoal ].valor() );
    }

    bool valido()
    {
        return mFilas != 0 || mColumnas != 0;
    }

private:

    int puedoMover( Casillero posIniCaja, Direccion d, Casillero posIni )
    {
        vector< UInt > pasos( mAlmac.getCantNodos() );

        int posActual = posIni;
        pasos[ posActual ] = 0;

        vector< Casillero > nodosAVisitar;
        for( Casillero nodoAct = 0; nodoAct < mAlmac.getCantNodos(); nodoAct++ )
        {
            nodosAVisitar.push_back( nodoAct );
        }

        while( !nodosAVisitar.empty() )
        {
            Casillero nodoAct;
            UInt pasosNodoAct;

            vector< Casillero >::const_iterator nodoMenor;
            for( vector< Casillero >::const_iterator it = nodosAVisitar.begin(); it < nodosAVisitar.end(); it++ )
            {
                if( pasosNodoAct >  pasos[ *it ] )
                {
                    nodoMenor = it;
                    pasosNodoAct = pasos[ *it ];
                }
            }

            if( pasosNodoAct.infinito() )
            {
                break;
            }

            nodoAct = *nodoMenor;
            nodosAVisitar.erase( nodoMenor );

            const vector< Almacen::Vecino >& vecinos = mAlmac.getVecinos( nodoAct );
            for( vector< Almacen::Vecino >::const_iterator itVec = vecinos.begin(); itVec < vecinos.end(); itVec++ )
            {
                if( itVec->vecino != posIniCaja && pasos[ itVec->vecino ] >  pasosNodoAct + 1 )
                {
                    pasos[ itVec->vecino ] = pasosNodoAct + 1;
                }
            }
        }

        const Casillero* posNecesaria = posParaEmpujar( posIniCaja, d );

        if( posNecesaria == NULL || pasos[ *posNecesaria ].infinito() )
        {
            return -1;
        }


        return pasos[ *posNecesaria ].valor();
    }

    const Casillero* posParaEmpujar( Casillero posIniCaja, Direccion d )
    {
        return mAlmac.getCasVecino( posIniCaja, dirContraria( d ) );
    }

protected:

    // cantidad de filas del almacén
    int mFilas;

    // cantidad de columnas del almacén
    int mColumnas;

    // posición actual de la caja
    Casillero mPosInicialCaja;

    // destino donde llegar la caja
    Casillero mGoal;

    // posición actual del cuidador del almacén (wharehouse keeper)
    Casillero mPosInicial;

    // mAlmac representativo del almacén
    Almacen mAlmac;
};

#define FILEINPUT

void main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );

    TestCase testActual;
    entrada >> testActual;
    int numTest = 1;
    while( testActual.valido() )
    {
        pair< int, int > res = testActual.resolver();

        if( res.first >= 0 )
        {
            salida << "Instancia " << numTest << endl;
            salida << res.first << " " << res.second << endl << endl;
        }
        else
        {
            salida << "Instancia " << numTest << endl;
            salida << "Impossivel" << endl << endl;
        }

        entrada >> testActual;
        numTest++;
    }

    salida.close();
    entrada.close();

#else

    TestCase testActual;
    cin >> testActual;
    int numTest = 1;
    while( testActual.valido() )
    {
        pair< int, int > res = testActual.resolver();

        if( res.first >= 0 )
        {
            cout << "Instancia " << numTest;
            cout << res.first << " " << res.second << endl;
        }
        else
        {
            cout << "Instancia " << numTest;
            cout << "Impossivel" << endl << endl;
        }

        cin >> testActual;
        numTest++;
    }

#endif
}