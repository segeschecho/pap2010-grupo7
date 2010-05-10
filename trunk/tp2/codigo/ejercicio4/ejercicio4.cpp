#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
using namespace std;

typedef unsigned int uint;
typedef uint Nodo;

inline int max( int a, int b ) { return a > b ? a : b; }
inline int min( int a, int b ) { return a < b ? a : b; }

class UnionFind
{
public:

    struct Data
    {
        int name;
        int size;
        struct Data *home;
    };

    UnionFind( int n ) : S( n )
    {
        for( int i = 0; i < n; i++ )
        {
            S[i].name = i + 1;
            S[i].size = 0;

            S[i].home = &S[i];
        }
    }

    void myunion( int a, int b )
    {
        int sizea, sizeb;
        sizea = S[a].home->size;
        sizeb = S[b].home->size;
        
        if( sizea > sizeb )
        {
            (S[b].home)->home = S[a].home;
            S[a].size = sizea + sizeb;
        }
        else
        {
            (S[a].home)->home = S[b].home;
            S[b].size = sizea + sizeb;
        }
    }

    int myfind(int a)
    {
        Data *temp, *temp2, *stoppoint;
        int result;
        temp2 = &S[a];
        temp = S[a].home;

        while( temp2 != temp )
        {
            temp2 = temp;
            temp = temp2->home;
        }

        result = temp2->name;
        stoppoint = temp2;
        temp2 = &S[a];
        
        // path compression
        do
        {
            temp = temp2->home;
            temp2->home = stoppoint;
            temp2 = temp;
        }while( temp2 != stoppoint );

        return result;
    }

protected:
    vector< Data > S;
};

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
        uint w;
    };

    virtual void agregarArista( Nodo nodo1, Nodo nodo2, uint costo ) = 0;
    virtual uint getCantNodos() const = 0;
};

class GrafoAristas : Grafo
{
public:

    GrafoAristas() : cantNodos( 0 ){};
    ~GrafoAristas(){};

    void agregarArista( Nodo nodo1, Nodo nodo2, uint costo )
    {
        cantNodos = max( nodo1 + 1, max( nodo2 + 1, cantNodos ) );
        listaAristas.push_back( Arista( nodo1, nodo2, costo ) );
    }

    uint getCantNodos() const
    {
        return cantNodos;
    }

    const vector< Arista >& getAristas() const
    {
        return listaAristas;
    }

protected:
    uint cantNodos;
    vector< Arista > listaAristas;
};

class GrafoAdyacentes : Grafo
{
public:

    GrafoAdyacentes( uint n ) : matrizAdyacencias( vector< vector< int > > ( n, vector< int > ( n, 0 ) ) ){};
    ~GrafoAdyacentes(){};

    inline void agregarArista( Nodo nodo1, Nodo nodo2, uint costo )
    {
        if( listaAdyacencias.size() <= nodo1 || listaAdyacencias.size() <= nodo2 )
        {
            listaAdyacencias.resize( max( nodo1, nodo2) + 1 );
        }
        listaAdyacencias[ nodo1 ].push_back( Grafo::Arista( nodo1, nodo2, costo ) );
        listaAdyacencias[ nodo2 ].push_back( Grafo::Arista( nodo2, nodo1, costo ) );

        matrizAdyacencias[ nodo1 ][ nodo2 ] = costo;
        matrizAdyacencias[ nodo2 ][ nodo1 ] = costo;
    }

    inline uint getCantNodos() const
    {
        return matrizAdyacencias.size();
    }

    inline const vector< Grafo::Arista >& getAristas( Nodo nodo ) const
    {
        return listaAdyacencias[ nodo ];
    }

    inline int getCostoArista( Nodo nodo1, Nodo nodo2 ) const
    {
        return matrizAdyacencias[ nodo1 ][ nodo2 ];
    }

protected:
    vector< vector< Arista > > listaAdyacencias;
    vector< vector< int > > matrizAdyacencias;
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

                t.mGrafo.agregarArista( --nodo1, --nodo2, costo );
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

        vector< Grafo::Arista > aristasGrafo = mGrafo.getAristas();
        vector< Grafo::Arista > aristasRestantes;

        uint n = mGrafo.getCantNodos(), cantAristasAgregadas = 0;
        sort( aristasGrafo.begin(), aristasGrafo.end() );

        UnionFind uf( n );
        GrafoAdyacentes mst( n );
        while( !aristasGrafo.empty() && cantAristasAgregadas < ( n - 1 ) )
        {
            vector< Grafo::Arista >::iterator a = aristasGrafo.begin();
            if( uf.myfind( a->vi ) != uf.myfind( a->vj ) )
            {
                uf.myunion( a->vi, a->vj );
                
                mst.agregarArista( a->vi, a->vj, a->w );
                res.first += a->w;
                cantAristasAgregadas++;
            }
            else
            {
                aristasRestantes.push_back( *a );
            }

            aristasGrafo.erase( a );
        }

        while( !aristasGrafo.empty() )
        {
            aristasRestantes.push_back( aristasGrafo[ 0 ] );
            aristasGrafo.erase( aristasGrafo.begin() );
        }

        res.second = -1;
        for( vector< Grafo::Arista >::iterator a = aristasRestantes.begin(); a < aristasRestantes.end(); a++ )
        {
            res.second = res.second == -1 ? buscarMstAgregando( *a, mst, res.first ) : min( res.second, buscarMstAgregando( *a, mst, res.first ) );
        }
        return res;
    }

private:
    int buscarMstAgregando( Grafo::Arista& aristaAgregada, const GrafoAdyacentes& g, int valorMst ) const
    {
        int res = -1;
        vector< Grafo::Arista >& camino = buscarCamino( aristaAgregada.vi, aristaAgregada.vj, g );
        for( vector< Grafo::Arista >::iterator a = camino.begin(); a < camino.end(); a++ )
        {
            res = res == -1 ? valorMst + aristaAgregada.w - a->w : min( res, valorMst + aristaAgregada.w - a->w );
        }

        return res;
    }

    vector< Grafo::Arista > buscarCamino( Nodo nodo1, Nodo nodo2, const GrafoAdyacentes& g ) const
    {
        uint n = g.getCantNodos();
        vector< Nodo > predecesor( n );

        vector< Nodo > cola;
        for( Nodo i = 0; i < n; i++ )
        {
            if( i == nodo1 )
            {
                cola.push_back( i );
            }
            predecesor[ i ] = i;
        }

        for( uint i = 0; i < n && predecesor[ nodo2 ] == nodo2; i++ )
        {
            const vector< Grafo::Arista >& aristasActual = g.getAristas( cola[ i ] );
            for( vector< Grafo::Arista >::const_iterator a = aristasActual.begin(); a < aristasActual.end(); a++ )
            {
                if( predecesor[ a->vj ] == a->vj && a->vj != nodo1 )
                {
                    cola.push_back( a->vj );
                    predecesor[ a->vj ] = a->vi;
                }
            }
        }

        vector< Grafo::Arista > res;
        for( uint i = nodo2; i != nodo1; )
        {
            res.push_back( Grafo::Arista( predecesor[ i ], i, g.getCostoArista(predecesor[ i ], i) ) );
            i = predecesor[ i ];
        }

        return res;
    }

protected:

    // grafo representativo de las escuelas con las conecciones
    // disponibles con sus correspondientes costos
    GrafoAristas mGrafo;
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
