#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;

class Int
{
public:
    Int(){ mInfinite = true; }
    Int( int valor ){ mInfinite = false; mEntero = valor; }
    void operator=( int valor ) { mInfinite = false; mEntero = valor; }
    void operator=( Int int2 ) { mInfinite = int2.mInfinite; mEntero = int2.mEntero; }
    bool operator>( Int int2 ) { if( !mInfinite && !int2.mInfinite ){ return mEntero > int2.mEntero; } return mInfinite && !int2.mInfinite; }
    Int operator+( int valor ) { if( !mInfinite ) { return mEntero + valor; } return Int(); }
    bool infinito() { return mInfinite; }
    int valor() { return mEntero; }
private:
    bool mInfinite;
    int mEntero;
};


class Red
{
public:

    class Eje
    {
    public:
        Eje( uint fuente, uint destino )
        {
            mFuente = fuente;
            mDestino = destino;
        }

        uint mFuente;
        uint mDestino;
    };

    typedef vector< Eje > VEje;

    Red( uint cantidadNodos )
    {
        mListaAdyacencias.resize( cantidadNodos );
        mCostoEje = new uint* [ cantidadNodos ];
        for( uint i = 0; i < cantidadNodos; i++ )
        {
            mCostoEje[ i ] = new uint [ cantidadNodos ];
            for( uint j = 0; j < cantidadNodos; j++ )
            {
                mCostoEje[ i ][ j ] = 0;
            }
        }

        mFlujoEje = new int* [ cantidadNodos ];
        for( uint i = 0; i < cantidadNodos; i++ )
        {
            mFlujoEje[ i ] = new int [ cantidadNodos ];
            for( uint j = 0; j < cantidadNodos; j++ )
            {
                mFlujoEje[ i ][ j ] = 0;
            }
        }
    }

    ~Red()
    {
        for( uint i = 0; i < mListaAdyacencias.size(); i++ )
        {
            delete mCostoEje[ i ];
        }
        delete mCostoEje;

        for( uint i = 0; i < mListaAdyacencias.size(); i++ )
        {
            delete mFlujoEje[ i ];
        }
        delete mFlujoEje;
    }

    // agrega al grafo un eje dirigido de nodo1 -> nodo2
    void agregarEje( uint nodo1, uint nodo2, uint costo, int flujo = 0 )
    {
        uint ultimoNodo = mListaAdyacencias.size() - 1;
        if( nodo1 > ultimoNodo || nodo2 > ultimoNodo )
        {
            mListaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        mListaAdyacencias[ nodo1 ].push_back( Eje( nodo1, nodo2 ) );
        mCostoEje[ nodo1 ][ nodo2 ] = costo;
        mFlujoEje[ nodo1 ][ nodo2 ] = flujo;
    }

    void setFlujoEje( uint fuente, uint destino, int flujo )
    {
        mFlujoEje[ fuente ][ destino ] = flujo;
    }

    int getFlujoEje( uint fuente, uint destino )
    {
        return mFlujoEje[ fuente ][ destino ];
    }

    int getFlujoEje( const Eje& eje )
    {
        return mFlujoEje[ eje.mFuente ][ eje.mDestino ];
    }

    uint getCostoEje( uint fuente, uint destino )
    {
        return mCostoEje[ fuente ][ destino ];
    }

    uint getCostoEje( const Eje& eje )
    {
        return mCostoEje[ eje.mFuente ][ eje.mDestino ];
    }

    const VEje& getEjesSalida( int nodo )
    {
        return mListaAdyacencias[ nodo ];
    }

    uint getCantNodos()
    {
        return mListaAdyacencias.size();
    }

protected:
    vector< VEje > mListaAdyacencias;

    // matriz de costo de cada eje
    uint** mCostoEje;

    // matriz de flujo de cada eje
    int** mFlujoEje;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // obtengo el k correspondiente al enunciado
        int cantNodos;
        if( is >> cantNodos )
        {
            t.mRed = new Red( cantNodos );
            int cantEjes;
            is >> cantEjes;
            for( int eje = 0; eje < cantEjes; eje++ )
            {
                uint nodo1, nodo2, costo;
                is >> nodo1;
                is >> nodo2;
                is >> costo;

                // en la representación interna el mRed tiene nodo 0, por lo que debemos
                // restarle 1 a los nodos pasados por parametro
                nodo1--;
                nodo2--;
                t.mRed->agregarEje( nodo1, nodo2, costo );
                t.mRed->agregarEje( nodo2, nodo1, costo );
            }

            is >> t.mCantidadData;
            is >> t.mCapacidadEnlaces;
        }

        return is;
    }
public:
    TestCase(){};
    ~TestCase()
    {
        delete mRed;
    }

    long long int resolver()
    {
        // inicializamos la matriz de adyacencias auxiliar
        int cantidadNodosGrafo = mRed->getCantNodos();

        int cantDataEnviada = 0;

        // Hacemos Ford Fulkerson
        while( cantDataEnviada < mCantidadData && buscarCaminoYAumentar( 0, cantidadNodosGrafo - 1 ) )
        {
            // actualizamos la cantidad de datos enviados hasta el momento
            cantDataEnviada += mCapacidadEnlaces;
        }

        if( cantDataEnviada < mCantidadData )
        {
            return -1;
        }

        long long int tiempoTotal = 0;
        // Ahora calculamos el tiempo que nos costó enviar los datos de un punto a otro
        for( int nodo = 0; nodo < cantidadNodosGrafo; nodo++ )
        {
            const Red::VEje& ejesSalida = mRed->getEjesSalida( nodo );
            for( Red::VEje::const_iterator eje = ejesSalida.begin(); eje < ejesSalida.end(); eje++ )
            {
                int flujo = mRed->getFlujoEje( *eje );
                if( flujo > 0 )
                {
                    tiempoTotal += mRed->getCostoEje( *eje ) * flujo;
                }
            }
        }

        return tiempoTotal;
    }

private:

    bool buscarCaminoYAumentar( int s, int t )
    {
        vector< pair< Int, Int > > caminosMinimos;
        buscarCaminoMinimo( s, caminosMinimos );

        // si se encontro un camino hasta el final
        if( !caminosMinimos[ t ].first.infinito() )
        {
            int nodoActual = t;

            // marco los ejes que use para este camino
            while( nodoActual != s )
            {
                int fuente = caminosMinimos[ nodoActual ].first.valor();

                int nuevoFlujo = mRed->getFlujoEje( fuente, nodoActual ) + min( mCapacidadEnlaces, mCantidadData );
                mRed->setFlujoEje( fuente, nodoActual, nuevoFlujo );

                nuevoFlujo = mRed->getFlujoEje( nodoActual, fuente ) - min( mCapacidadEnlaces, mCantidadData );
                mRed->setFlujoEje( nodoActual, fuente, nuevoFlujo );
                nodoActual = fuente;
            }

            return !caminosMinimos[ t ].second.infinito();
        }
        else
        {
            return false;
        }
    }

    // Este algoritmo es una implementación de Bellman-Ford.
    // Resultado es un vector de pares, que al finalizar la ejecución del algoritmo
    // para i = 0, ..., n, siendo n la cantidad de nodos de la red,
    // resultado[ i ].first representa el predecesor inmediato del camino de menor costo
    // desde s hasta el nodo i, y resultado[ i ].second representa el costo acumulado para
    // el camino de menor costo desde s hasta el nodo i.
    void buscarCaminoMinimo( int s, vector< pair< Int, Int > >& resultado )
    {
        uint cantNodos = mRed->getCantNodos();

        if( resultado.size() < cantNodos )
        {
            resultado.resize( cantNodos );
        }

        resultado[ s ].second = 0;

        bool resultadoCambio = true;
        for( uint i = 0; i < cantNodos && resultadoCambio; i++ )
        {
            resultadoCambio = false;
            for( uint nodo = 0; nodo < cantNodos; nodo++ )
            {
                const Red::VEje& ejesSalida = mRed->getEjesSalida( nodo );
                for( Red::VEje::const_iterator eje = ejesSalida.begin(); eje < ejesSalida.end(); eje++ )
                {
                    Int distanciaFuente = resultado[ eje->mFuente ].second;
                    Int distanciaDestino = resultado[ eje->mDestino ].second;
                    uint costoEje = mRed->getCostoEje( *eje );
                    int flujoEje = mRed->getFlujoEje( *eje );
                    if( flujoEje < mCapacidadEnlaces && distanciaDestino > distanciaFuente + costoEje )
                    {
                        resultado[ eje->mDestino ].second = distanciaFuente + costoEje;
                        resultado[ eje->mDestino ].first = eje->mFuente;
                        resultadoCambio = true;
                    }
                }
            }
        }
    }

    // representa la cantidad de datos a enviar al otro punto de la red
    int mCantidadData;

    // capacidad de los links de la red
    int mCapacidadEnlaces;

    // mRed representativo de la red
    Red* mRed;
};

int dataflow( istream& entrada, ostream& salida )
{
    for(TestCase testActual; entrada >> testActual; )
    {
        long long int resultado = testActual.resolver();
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
{
    
    ifstream in( "test2", ios_base::in );
    ofstream out( "test2Out", ios_base::out );

    int ret = dataflow( in, out );

    in.close();
    out.close();

    return ret;
}
