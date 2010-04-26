#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;

inline int max( int a, int b )
{
    if( a > b )
        return a;
    return b;
}

inline int min( int a, int b )
{
    if( a > b )
        return b;
    return a;
}

class Grafo
{
public:

    typedef uint Sucesor;
    typedef uint Peso;
    typedef pair< Sucesor, Peso > Eje;
    typedef vector< Eje > EjesConPesos;

    Grafo(){};

    Grafo( int cantidadNodos )
    {
        setCantidadNodos( cantidadNodos );
    }
    ~Grafo(){};

    void agregarEje( uint nodo1, uint nodo2, uint peso )
    {
        uint ultimoNodo = listaAdyacencias.size() - 1;
        if( nodo1 > ultimoNodo || nodo2 > ultimoNodo )
        {
            listaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        listaAdyacencias[ nodo1 ].push_back( pair< Sucesor, Peso >( nodo2, peso ) );
        listaAdyacencias[ nodo2 ].push_back( pair< Sucesor, Peso >( nodo1, peso ) );
    }

    const EjesConPesos& getAdyacentes( int nodo )
    {
        return listaAdyacencias[ nodo ];
    }

    uint getCantNodos()
    {
        return listaAdyacencias.size();
    }

    void setCantidadNodos( int cantidadNodos )
    {
        listaAdyacencias.resize( cantidadNodos );
    }

    void clear()
    {
        listaAdyacencias.clear();
    }

private:
    vector< EjesConPesos > listaAdyacencias;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // primero limpio todas las variables miembro de t
        t.mGrafo.clear();

        // obtengo el k correspondiente al enunciado
        int cantNodos;
        if( is >> cantNodos )
        {
            int cantEjes;
            is >> cantEjes;
            t.mGrafo.setCantidadNodos( cantNodos );
            for( int eje = 0; eje < cantEjes; eje++ )
            {
                int nodo1, nodo2, peso;
                is >> nodo1;
                is >> nodo2;
                is >> peso;

                // en la representación interna el mGrafo tiene nodo 0, por lo que debemos
                // restarle 1 a los nodos pasados por parametro
                nodo1--;
                nodo2--;
                t.mGrafo.agregarEje( nodo1, nodo2, peso );
            }

            is >> t.mCantidadData;
            is >> t.mCapacidadEnlaces;
        }

        return is;
    }
public:
    TestCase(){};
    ~TestCase(){};

    int resolver()
    {
        // inicializamos la matriz de adyacencias auxiliar
        int cantidadNodosGrafo = mGrafo.getCantNodos();
        ejeUsado = new bool* [ cantidadNodosGrafo ];
        for( int i = 0; i < cantidadNodosGrafo; i++ )
        {
            ejeUsado[ i ] = new bool [ cantidadNodosGrafo ];
            for( int j = 0; j < cantidadNodosGrafo; j++ )
            {
                ejeUsado[ i ][ j ] = false;
            }
        }

        int tiempoTotal = 0;
        int cantDataEnviada = 0;
        // Hacemos algo parecido a Ford Fulkerson
        int cantTiempoUsado = buscarYEliminarCaminoMinimo( 0, cantidadNodosGrafo - 1 );
        while( cantDataEnviada < mCantidadData && cantTiempoUsado >= 0 )
        {
            // sumamos el tiempo que se tarda en recorrer el camino encontrado
            tiempoTotal += cantTiempoUsado * min( mCapacidadEnlaces, mCantidadData - cantDataEnviada );

            // actualizamos la cantidad de datos restantes
            cantDataEnviada += mCapacidadEnlaces;

            // buscamos un nuevo camino para el siguiente ciclo
            cantTiempoUsado = buscarYEliminarCaminoMinimo( 0, cantidadNodosGrafo - 1 );
        }

        if( cantDataEnviada < mCantidadData )
        {
            return -1;
        }

        return tiempoTotal;
    }

private:

    int buscarYEliminarCaminoMinimo( int s, int t )
    {
        vector< pair< int, int > > caminosMinimos;
        buscarCaminoMinimo( s, caminosMinimos );

        // si se encontro un camino hasta el final
        if( caminosMinimos[ t ].first != -1 )
        {
            int nodoActual = t;

            // marco los ejes que use para este camino
            while( nodoActual != s )
            {
                int predecesor = caminosMinimos[ nodoActual ].first;
                ejeUsado[ nodoActual ][ predecesor ] = true;
                ejeUsado[ predecesor ][ nodoActual ] = true;
                nodoActual = predecesor;
            }

            return caminosMinimos[ t ].second;
        }
        else
        {
            return -1;
        }
    }

    void buscarCaminoMinimo( int s, vector< pair< int, int > >& resultado )
    {
        // asignamos a cada nodo un valor de distancia 0 para el nodo inicial y -1 para todos los demas.
        uint ultimoNodo = mGrafo.getCantNodos() - 1;

        if( resultado.size() < ultimoNodo + 1 )
        {
            resultado.resize( ultimoNodo + 1 );
        }

        for( uint nodo = 0; nodo < ultimoNodo + 1; nodo++ )
        {
            resultado[ nodo ].first = -1;
            resultado[ nodo ].second = -1;
        }

        // marcamos todos los nodos como no visitados
        vector< bool > nodoVisitado( ultimoNodo + 1, false );
        resultado[ s ].second = 0;

        // nodosAVisitar representa los nodos que falta visitar. Se inicializa con
        // todos los nodos del mGrafo
        vector< uint > nodosAVisitar( ultimoNodo + 1 );
        for(uint i = 0; i < ultimoNodo + 1; i++ )
        {
            nodosAVisitar[i] = i;
        }

        // si todos los nodos ya fueron visitados, terminamos
        while( !nodosAVisitar.empty() )
        {
            // buscamos el nodo no visitado que tenga menor distancia desde el inicio
            vector< uint >::iterator itNodoActual = nodosAVisitar.begin();
            int distNodoActual = resultado[ *itNodoActual ].second;

            for( vector< uint >::iterator it = nodosAVisitar.begin(); it < nodosAVisitar.end(); it++ )
            {
                if( distNodoActual > resultado[ *it ].second && resultado[ *it ].second >= 0 )
                {
                    distNodoActual = resultado[ *it ].second;
                    itNodoActual = it;
                }
            }

            // lo seteamos como nodo actual y lo marcamos como visitado
            int nodoActual = *itNodoActual;
            nodosAVisitar.erase( itNodoActual );
            nodoVisitado[ nodoActual ] = true;

            // consideramos todos sus sucesores y actualizamos la distancia y predecesor
            const Grafo::EjesConPesos& sucesores = mGrafo.getAdyacentes( nodoActual );
            for( Grafo::EjesConPesos::const_iterator itSucesores = sucesores.begin(); itSucesores < sucesores.end(); itSucesores++ )
            {
                int sucesor = itSucesores->first;
                int peso = itSucesores->second;
                if( !ejeUsado[ nodoActual ][ sucesor ] && \
                    !nodoVisitado[ sucesor ]  && \
                    ( resultado[ sucesor ].second > resultado[ nodoActual ].second + peso || resultado[ sucesor ].second == -1 ) )
                {
                    resultado[ sucesor ].second = resultado[ nodoActual ].second + peso;
                    resultado[ sucesor ].first = nodoActual;
                }
            }
        }
    }

    // es auxiliar, representa los ejes que van siendo usados y descartados para la siguiente iteracion
    // durante el calculo del camino de aumento
    bool** ejeUsado;

    // representa la cantidad de datos a enviar al otro punto de la red
    int mCantidadData;

    // capacidad de los links de la red
    int mCapacidadEnlaces;

    // mGrafo representativo de la red
    Grafo mGrafo;
};

int dataflow( istream& entrada, ostream& salida )
{
    TestCase testActual;
    while( entrada >> testActual )
    {
        int resultado = testActual.resolver();
        if( resultado != -1 )
        {
            salida << resultado << endl;
        }
        else
        {
            salida << "Impossible." << endl;
        }
    }

    return 0;
}

int main()
{/*
    ifstream in( "test", ios_base::in );
    ofstream out( "testOut", ios_base::out );
*/
    int ret = dataflow( cin, cout );
/*
    in.close();
    out.close();
*/
    return ret;
}
