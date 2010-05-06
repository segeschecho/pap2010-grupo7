#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

typedef unsigned int uint;
typedef uint Nodo;

inline int max( int a, int b )
{
    if( a > b )
        return a;
    return b;
}

class Grafo
{
public:
    Grafo(){};

    Grafo( int cantidadNodos ) : listaAdyacencias( cantidadNodos ){};
    ~Grafo(){};

    void agregarEje( Nodo nodo1, Nodo nodo2 )
    {
        if( nodo1 >= listaAdyacencias.size() || nodo2 >= listaAdyacencias.size() )
        {
            listaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        listaAdyacencias[ nodo1 ].push_back( nodo2 );
        listaAdyacencias[ nodo2 ].push_back( nodo1 );
    }

    const vector< Nodo >& getAdyacentes( Nodo nodo )
    {
        return listaAdyacencias[ nodo ];
    }

    Nodo cantNodos()
    {
        return listaAdyacencias.size();
    }

    void agregarNodo()
    {
        listaAdyacencias.resize( listaAdyacencias.size() + 1 );
    }

    void clear()
    {
        listaAdyacencias.clear();
    }

private:
    vector< vector< Nodo > > listaAdyacencias;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // primero limpio todas las variables miembro de t
        t.mFilas = 0;
        t.mColumnas = 0;
        t.mPosCaja = -1;
        t.mGoal = -1;
        t.mPosKeeper = -1;
        t.mGrafo.clear();

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

            // este es el mapa de casillero en la matriz a numero de nodo
            map< int, Nodo > nodoDeCasillero;

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
                        t.mGrafo.agregarNodo();
                        nodoDeCasillero.insert( pair<int, Nodo>( fila*t.mColumnas + col, ultimoNodo ) );

                        // agregar un eje al nodo de la izquierda, si es que existe y no hay pared
                        if( col > 0 && !hayPared[ fila ][ col - 1 ] )
                        {
                            t.mGrafo.agregarEje( ultimoNodo, ultimoNodo - 1 );
                        }

                        // agregar un eje al nodo de arriba, si es que existe y no hay pared
                        if( fila > 0 && !hayPared[ fila - 1 ][ col ] )
                        {
                            t.mGrafo.agregarEje( ultimoNodo, nodoDeCasillero[ (fila - 1)*t.mColumnas + col ] );
                        }

                        if( c == 'T' )
                        {
                            t.mGoal = ultimoNodo;
                        }
                        else
                        {
                            if( c == 'B' )
                            {
                                t.mPosCaja = ultimoNodo;
                            }
                            else
                            {
                                if( c == 'S' )
                                {
                                    t.mPosKeeper = ultimoNodo;
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

    bool resolver()
    {
        return false;
    }

    bool valido()
    {
        return mFilas != 0 || mColumnas != 0;
    }

private:

    // cantidad de filas del almacén
    int mFilas;

    // cantidad de columnas del almacén
    int mColumnas;

    // posición actual de la caja
    int mPosCaja;

    // destino donde llegar la caja
    int mGoal;

    // posición actual del cuidador del almacén (wharehouse keeper)
    int mPosKeeper;

    // mGrafo representativo del almacén
    Grafo mGrafo;
};

#define FILEINPUT

void main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );

    TestCase testActual;
    entrada >> testActual;
    while( testActual.valido() )
    {
        if( testActual.resolver() )
        {
            salida << "YES" << endl;
        }
        else
        {
            salida << "NO" << endl;
        }

        entrada >> testActual;
    }

    salida.close();
    entrada.close();

#else

    TestCase testActual;
    cin >> testActual;
    while( testActual.valido() )
    {
        if( testActual.resolver() )
        {
            cout << "YES" << endl;
        }
        else
        {
            cout << "NO" << endl;
        }

        cin >> testActual;
    }

#endif
}