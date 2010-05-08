#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
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
    class Arista
    {
    public:
        Arista( Nodo nodo1, Nodo nodo2, uint costo ) : vi( nodo1 ), vj( nodo2 ), w( costo ){};
        bool operator<( const Arista& otraArista ) const { return this->w < otraArista.w; }
        bool operator!=( const Arista& otraArista ) const { return !( *this == otraArista ); }
        bool operator==( const Arista& otraArista ) const { return this->vi == otraArista.vi && this->vj == otraArista.vj; }

        Nodo vi;
        Nodo vj;
        int w;
    };

    Grafo(){};
    ~Grafo(){};

    void agregarEje( Nodo nodo1, Nodo nodo2, uint costo )
    {
        if( nodo1 >= listaAdyacencias.size() || nodo2 >= listaAdyacencias.size() )
        {
            listaAdyacencias.resize( max( nodo1, nodo2 ) + 1 );
        }

        listaAristas.push_back( Arista( nodo1, nodo2, costo ) );

        listaAdyacencias[ nodo1 ].push_back( Arista( nodo1, nodo2, costo ) );
        listaAdyacencias[ nodo2 ].push_back( Arista( nodo2, nodo1, costo ) );
    }

    uint getCantNodos()
    {
        return listaAdyacencias.size();
    }

    const vector< Arista >& getAristas()
    {
        return listaAristas;
    }

    const vector< Arista >& getAristas( Nodo nodo )
    {
        return listaAdyacencias[ nodo ];
    }

protected:
    vector< Arista > listaAristas;
    vector< vector< Arista > > listaAdyacencias;
};

class TestCase
{
    friend istream& operator>>( istream& is, TestCase& t )
    {
        uint cantEscuelas;
        if( is >> cantEscuelas )
        {
            uint cantConecciones;
            is >> cantConecciones;
            for( uint conecc = 0; conecc < cantConecciones; conecc++ )
            {
                Nodo nodo1, nodo2;
                uint costo;

                is >> nodo1 >> nodo2 >> costo;

                t.mGrafo.agregarEje( --nodo1, --nodo2, costo );
            }
        }

        return is;
    }

public:
    TestCase(){};
    ~TestCase(){};

    pair< int, int > resolver()
    {
        pair< int, int > res;
        res.first = 0;
        res.second = 0;

        vector< Grafo::Arista > aristas = mGrafo.getAristas();
        sort( aristas.begin(), aristas.end() );

        uint n = mGrafo.getCantNodos();
        vector< uint > ciclos;
        for( uint i = 0; i < n; i++ )
        {
            ciclos.push_back( i );
        }

        vector< Grafo::Arista > aristasAgregadas;
        vector< uint > gradoNodoEnAG ( n, 0 );

        while( aristasAgregadas.size() < (n - 1) && aristas.size() )
        {
            if( ciclos[ aristas[ 0 ].vi ] != ciclos[ aristas[0].vj ] )
            {
                uint cantCiclos = ciclos[ aristas[ 0 ].vj ];
                for( Nodo i = 0; i < n ; i++ )
                {
                    if( ciclos[ i ] == cantCiclos )
                    {
                        ciclos[ i ] = ciclos[ aristas[ 0 ].vi ];
                    }
                }

                res.first += aristas[ 0 ].w;
                gradoNodoEnAG[ aristas[ 0 ].vi ]++;
                gradoNodoEnAG[ aristas[ 0 ].vj ]++;
                aristasAgregadas.push_back( aristas[ 0 ] );
            }

            aristas.erase(aristas.begin(), aristas.begin() + 1);
        }

        if( aristas.size() > 0 )
        {
            res.second = res.first + aristas[ 0 ].w;
            for( vector< Grafo::Arista >::const_iterator a = aristasAgregadas.begin(); a < aristasAgregadas.end(); a++ )
            {
                if( gradoNodoEnAG[ a->vi ] > 1 && gradoNodoEnAG[ a->vj ] > 1 )
                {
                    vector< Grafo::Arista > aristasVi = mGrafo.getAristas( a->vi );
                    for( vector< Grafo::Arista >::const_iterator it = aristasVi.begin(); it < aristasVi.end(); it++ )
                    {
                        if( *it != *a && res.second > res.first - a->w + it->w )
                        {
                            res.second = res.first - a->w + it->w;
                        }
                    }

                    vector< Grafo::Arista > aristasVj = mGrafo.getAristas( a->vj );
                    for( vector< Grafo::Arista >::const_iterator it = aristasVj.begin(); it < aristasVj.end(); it++ )
                    {
                        if( *it != *a && res.second > res.first - a->w + it->w )
                        {
                            res.second = res.first - a->w + it->w;
                        }
                    }
                }
            }
        }
        else
        {
            res.second = res.first;
        }

        return res;
    }

protected:

    // grafo representativo de las escuelas con las conecciones
    // disponibles con sus correspondientes costos
    Grafo mGrafo;
};

#define FILEINPUT

int main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );

    uint cantidadTests;
    entrada >> cantidadTests;
    while( cantidadTests > 0 )
    {
        TestCase testActual;
        entrada >> testActual;
        pair< int, int > res = testActual.resolver();

        salida << res.first << " " << res.second << endl;

        cantidadTests--;
    }

    salida.close();
    entrada.close();

#else

    uint cantidadTests;
    cin >> cantidadTests;
    while( cantidadTests > 0 )
    {
        TestCase testActual;
        cin >> testActual;
        pair< int, int > res = testActual.resolver();

        cout << res.first << " " << res.second << endl;

        cantidadTests--;
    }

#endif
    return 0;
}
