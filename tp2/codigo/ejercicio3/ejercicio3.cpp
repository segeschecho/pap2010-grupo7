#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;
typedef long long int llint;
typedef uint Nodo;
typedef vector< vector< llint > > Matriz;

inline uint min( uint a, uint b ) { return a > b ? b : a; }
inline uint max( uint a, uint b ) { return a > b ? a : b; }
//-----------------------------------------------------------------------------------
// Clase ULLInt
//-----------------------------------------------------------------------------------

/*
 *  Esta clase representa un entero sin signo que posiblemente puede ser infinito.
 *  Se provee de los métodos mínimos y necesarios para resolver el ejercicio.
 */
class ULLInt
{
public:
    // Constructores
    //-----------------------------------------------------------------------------------
    ULLInt() : mInfinite ( true ) {};
    ULLInt( ullint valor ) : mInfinite( false ), mEntero( valor ){};

    // Operadores básicos
    //-----------------------------------------------------------------------------------
    void operator=( ullint valor ) { mInfinite = false; mEntero = valor; }
    void operator=( const ULLInt& int2 ) { mInfinite = int2.mInfinite; mEntero = int2.mEntero; }
    bool operator>( const ULLInt& int2 ) const { return ( !mInfinite && !int2.mInfinite ) ? mEntero > int2.mEntero : mInfinite && !int2.mInfinite; }
    ULLInt operator+( ullint valor ) const { return !mInfinite ? mEntero + valor : ULLInt(); }

    // Observadores
    //-----------------------------------------------------------------------------------
    bool infinito() const { return mInfinite; }
    ullint valor() const { return mEntero; }

private:
    // Atributos
    //-----------------------------------------------------------------------------------
    bool mInfinite;
    ullint mEntero;
};


//-----------------------------------------------------------------------------------
// Clase Red
//-----------------------------------------------------------------------------------
/*
 *  Esta clase representa un grafo con ejes dirigidos, cada uno con un costo y flujo asignados.
 *  No se le provee una capacidad a cada eje pues en el ejercicio tienen todos el mismo valor.
 *  En su lugar, se le provee de un atributo que indica la capacidad de los ejes.
 *  Se provee de los métodos mínimos y necesarios para resolver el ejercicio.
 *  
 */
class Red
{
public:

    //-----------------------------------------------------------------------------------
    // Eje
    struct Eje
    {
        Nodo mFuente;
        Nodo mDestino;
    };

    typedef vector< Eje > VEje;

    // Constructor y destructor
    //-----------------------------------------------------------------------------------
    Red( uint cantidadNodos ) :
        mListaAdyacencias( cantidadNodos ), 
        mCostoEje( cantidadNodos, vector< llint >( cantidadNodos ) ),
        mFlujoEje( cantidadNodos, vector< llint >( cantidadNodos ) )
    {
    }

    ~Red(){}

    //-----------------------------------------------------------------------------------
    // agrega al grafo un eje dirigido de nodo1 -> nodo2 con costo y flujo
    void agregarEje( Nodo nodo1, Nodo nodo2, llint costo, llint flujo = 0 )
    {
        Nodo ultimoNodo = mListaAdyacencias.size() - 1;
        if( nodo1 > ultimoNodo || nodo2 > ultimoNodo )
        {
            mListaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        Eje nuevoEje;
        nuevoEje.mDestino = nodo1;
        nuevoEje.mFuente = nodo2;
        mListaAdyacencias[ nodo1 ].push_back( nuevoEje );
        mCostoEje[ nodo1 ][ nodo2 ] = costo;
        mFlujoEje[ nodo1 ][ nodo2 ] = flujo;
    }

    //-----------------------------------------------------------------------------------
    void setFlujoEje( Nodo fuente, Nodo destino, llint flujo )
    {
        mFlujoEje[ fuente ][ destino ] = flujo;
    }

    llint getFlujoEje( Nodo fuente, Nodo destino )
    {
        return mFlujoEje[ fuente ][ destino ];
    }

    llint getFlujoEje( const Eje& eje )
    {
        return mFlujoEje[ eje.mFuente ][ eje.mDestino ];
    }

    //-----------------------------------------------------------------------------------
    llint getCostoEje( Nodo fuente, Nodo destino )
    {
        return mCostoEje[ fuente ][ destino ];
    }

    llint getCostoEje( const Eje& eje )
    {
        return mCostoEje[ eje.mFuente ][ eje.mDestino ];
    }

    //-----------------------------------------------------------------------------------
    const VEje& getEjesSalida( Nodo nodo )
    {
        return mListaAdyacencias[ nodo ];
    }

    //-----------------------------------------------------------------------------------
    uint getCantNodos()
    {
        return mListaAdyacencias.size();
    }

    //-----------------------------------------------------------------------------------
    ullint getCapacidadEjes()
    {
        return mCapacidadEjes;
    }

    void setCapacidadEjes( ullint valor )
    {
        mCapacidadEjes = valor;
    }
protected:
    // lista de adyacencias del grafo
    vector< VEje > mListaAdyacencias;

    // matriz de costo de cada eje
    Matriz mCostoEje;

    // matriz de flujo de cada eje
    Matriz mFlujoEje;

    // capacidad de los ejes
    ullint mCapacidadEjes;
};

//-----------------------------------------------------------------------------------
// Clase TestCase
//-----------------------------------------------------------------------------------
/*
 *  Esta clase representa un caso de test del ejercicio de data flow.
 *  Contiene la implementación de su resolución mediante Ford Fulkerson para encontrar
 *  el flujo máximo, y Bellman-Ford para encontrar los caminos de aumento de costo mínimo.
 *  Se provee de los métodos mínimos y necesarios para resolver el ejercicio.
 *  
 */
class TestCase
{
    //-----------------------------------------------------------------------------------
    friend istream& operator>>( istream& is, TestCase& t )
    {
        // obtengo el k correspondiente al enunciado
        uint cantNodos;
        if( is >> cantNodos )
        {
            t.mRed = new Red( cantNodos );
            uint cantEjes;
            is >> cantEjes;
            for( uint eje = 0; eje < cantEjes; eje++ )
            {
                Nodo nodo1, nodo2;
                ullint costo;
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
            ullint capacidadEjes;
            is >> capacidadEjes;
            t.mRed->setCapacidadEjes( capacidadEjes );
        }

        return is;
    }
public:
    //-----------------------------------------------------------------------------------
    TestCase(){};

    ~TestCase()
    {
        delete mRed;
    }

    //-----------------------------------------------------------------------------------
    // Este algoritmo es una implementación de Ford Fulkerson.
    llint resolver()
    {
        // inicializamos la cantidad de data enviada al otro punto de la red a 0
        mCantDataEnviada = 0;

        // inicializamos la matriz de adyacencias auxiliar
        uint cantidadNodosGrafo = mRed->getCantNodos();

        // Hacemos Ford Fulkerson
        llint aumento = buscarCaminoYAumentar( 0, cantidadNodosGrafo - 1 );
        while( mCantDataEnviada < mCantidadData && aumento >= 0 )
        {
            // actualizamos la cantidad de datos enviados hasta el momento
            mCantDataEnviada += aumento;
    
            // buscamos otro camino de aumento
            aumento = buscarCaminoYAumentar( 0, cantidadNodosGrafo - 1 );
        }

        if( mCantDataEnviada < mCantidadData )
        {
            return -1;
        }

        llint tiempoTotal = 0;
        // Ahora calculamos el tiempo que nos costó enviar los datos de un punto a otro
        for( Nodo nodo = 0; nodo < cantidadNodosGrafo; nodo++ )
        {
            const Red::VEje& ejesSalida = mRed->getEjesSalida( nodo );
            for( Red::VEje::const_iterator eje = ejesSalida.begin(); eje < ejesSalida.end(); eje++ )
            {
                llint flujo = mRed->getFlujoEje( *eje );
                if( flujo > 0 )
                {
                    tiempoTotal += mRed->getCostoEje( *eje ) * flujo;
                }
            }
        }

        return tiempoTotal;
    }

private:

    //-----------------------------------------------------------------------------------
    // Busca un camino de aumento y realiza el aumento del flujo correspondiente a ese camino.
    llint buscarCaminoYAumentar( Nodo s, Nodo t )
    {
        vector< pair< ULLInt, ULLInt > > caminosMinimos;
        buscarCaminoMinimo( s, caminosMinimos );

        // si se encontro un camino hasta el final
        if( !caminosMinimos[ t ].first.infinito() )
        {
            Nodo nodoActual = t;

            // aumento el flujo para el camino de aumento encontrado
            llint aumento = min( mRed->getCapacidadEjes(), mCantidadData - mCantDataEnviada );
            while( nodoActual != s )
            {
                Nodo fuente = caminosMinimos[ nodoActual ].first.valor();

                llint nuevoFlujo = mRed->getFlujoEje( fuente, nodoActual ) + aumento;
                mRed->setFlujoEje( fuente, nodoActual, nuevoFlujo );

                nuevoFlujo = mRed->getFlujoEje( nodoActual, fuente ) - aumento;
                mRed->setFlujoEje( nodoActual, fuente, nuevoFlujo );
                nodoActual = fuente;
            }

            return aumento;
        }

        return -1;
    }

    //-----------------------------------------------------------------------------------
    // Este algoritmo es una implementación de Bellman-Ford.
    // El parámetro resultado es un vector de pares, que al finalizar la ejecución del
    // algoritmo para i = 0, ..., n, siendo n la cantidad de nodos de la red,
    // resultado[ i ].first representa el predecesor inmediato del camino de menor costo
    // desde s hasta el nodo i, y resultado[ i ].second representa el costo acumulado para
    // el camino de menor costo desde s hasta el nodo i.
    void buscarCaminoMinimo( Nodo s, vector< pair< ULLInt, ULLInt > >& resultado )
    {
        uint cantNodos = mRed->getCantNodos();
        llint capacidadEjes = mRed->getCapacidadEjes();

        if( resultado.size() < cantNodos )
        {
            resultado.resize( cantNodos );
        }

        resultado[ s ].second = 0;

        bool resultadoCambio = true;
        for( uint i = 0; i < cantNodos && resultadoCambio; i++ )
        {
            resultadoCambio = false;
            for( Nodo nodo = 0; nodo < cantNodos; nodo++ )
            {
                const Red::VEje& ejesSalida = mRed->getEjesSalida( nodo );
                for( Red::VEje::const_iterator eje = ejesSalida.begin(); eje < ejesSalida.end(); eje++ )
                {
                    const ULLInt& distanciaFuente = resultado[ eje->mFuente ].second;
                    const ULLInt& distanciaDestino = resultado[ eje->mDestino ].second;
                    llint flujoEje = mRed->getFlujoEje( *eje );

                    ullint costoEje;
                    if( flujoEje >= 0 )
                    {
                        costoEje = mRed->getCostoEje( *eje );
                    }
                    else
                    {
                        costoEje = -mRed->getCostoEje( *eje );
                    }
                    if( flujoEje < capacidadEjes && distanciaDestino > distanciaFuente + costoEje )
                    {
                        resultado[ eje->mDestino ].second = distanciaFuente + costoEje;
                        resultado[ eje->mDestino ].first = eje->mFuente;
                        resultadoCambio = true;
                    }
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    // representa la cantidad de datos a enviar al otro punto de la red
    ullint mCantidadData;

    // representa la cantidad de datos ya enviados al otro punto de la red
    ullint mCantDataEnviada;

    // representa la red
    Red* mRed;
};

int main()
{
    for( TestCase testActual; cin >> testActual; )
    {
        llint resultado = testActual.resolver();
        if( resultado >= 0 )
        {
            cout << resultado;
        }
        else
        {
            cout << "Impossible.";
        }

        cout << endl;
    }

    return 0;
}