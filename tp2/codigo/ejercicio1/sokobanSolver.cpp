#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

inline int max( int a, int b )
{
    if( a > b )
        return a;
    return b;
}

class Grafo
{
public:
    Grafo()
    {
        ultimoNodo = -1;
    }

    Grafo(int cantidadNodos)
    {
        listaAdyacencias.resize( cantidadNodos );
    }
    ~Grafo(){};

    void agregarEje( int nodo1, int nodo2 )
    {
        if( nodo1 > ultimoNodo || nodo2 > ultimoNodo )
        {
            ultimoNodo = max( nodo1, nodo2 );
            listaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        listaAdyacencias[ nodo1 ].push_back( nodo2 );
        listaAdyacencias[ nodo2 ].push_back( nodo1 );
    }

    const vector< int >& getAdyacentes( int nodo )
    {
        return listaAdyacencias[ nodo ];
    }

    int getUltimoNodo()
    {
        return ultimoNodo;
    }

    void clear()
    {
        ultimoNodo = -1;
        listaAdyacencias.clear();
    }

private:
    vector< vector< int > > listaAdyacencias;
    int ultimoNodo;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // primero limpio todas las variables miembro de t
        t.k = -1;
        t.posCajas.clear();
        t.goals.clear();
        t.posKeeper = -1;
        t.grafo.clear();

        // obtengo el k correspondiente al enunciado
        if( is >> t.k )
        {
            int ultimoNodo = 0, primerNodoLineaActual = 0, nodoArriba = -1;

            // ignoro el caracter de '\n' que hay luego del k
            is.ignore();
            // FIXME: ARREGLAR ESTO EN EL MOMENTO DE LA ENTREGA PARA QUE SEA GETCHAR
            char c = is.peek();
            is.ignore();
           
            // parseamos caracter por caracter hasta encontrarnos con la línea que separa los casos de test
            while( c != -1 && !( is.peek() == '\n' && c == '\n' ) )
            {
                if( c == '\n' )
                {
                    nodoArriba = primerNodoLineaActual;
                    primerNodoLineaActual = ultimoNodo;
                }
                else
                {
                    if( c != '#' )
                    {
                        // si hay algun nodo a la izquierda del nodo actual
                        if( primerNodoLineaActual != ultimoNodo )
                        {
                            t.grafo.agregarEje( ultimoNodo, ultimoNodo - 1 );
                        }

                        // si no es la primer linea (es decir, si hay nodos arriba del nodo actual)
                        if( primerNodoLineaActual != 0 &&  nodoArriba != -1 )
                        {
                            t.grafo.agregarEje( ultimoNodo, nodoArriba );
                        }

                        if( c == '.' || c == '*' || c == '+' )
                        {
                            t.goals.push_back( ultimoNodo );
                        }

                        if( c == '$' || c == '*' )
                        {
                            t.posCajas.push_back( ultimoNodo );
                        }

                        if( c == '@' || c == '+' )
                        {
                            t.posKeeper = ultimoNodo;
                        }

                        ultimoNodo++;
                        if( nodoArriba != -1 )
                        {
                            nodoArriba++;
                            if( nodoArriba >= primerNodoLineaActual )
                            {
                                nodoArriba = -1;
                            }
                        }
                    }
                }

                c = is.peek();
                is.ignore();
            }

            // ignoro el caracter de nueva linea '\n'
            is.ignore();

            return is;
        }
        else
        {
            return is;
        }
    }
public:
    TestCase(){};
    ~TestCase(){};

    bool resolver()
    {
        return false;
    }

private:

    // máxima cantidad de movimientos
    int k;

    // posición actual de las cajas
    vector< int > posCajas;

    // destinos donde deben terminar las cajas
    vector< int > goals;

    // posición actual del cuidador del almacén (wharehouse keeper)
    int posKeeper;

    // grafo representativo del almacén
    Grafo grafo;
};

#define FILEINPUT

void main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );

    TestCase testActual;
    while( entrada >> testActual )
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
    while( !cin.eof() )
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