//-------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------

#include<vector>
#include<iostream>
#include <algorithm>

using namespace std;

//-------------------------------------------------------------------
//Intervalo Class
//-------------------------------------------------------------------
class Intervalo
{
public:
    //-------------------------------------------------------------------
    Intervalo( int L, int R ){ this->L = L; this->R = R; }
    ~Intervalo(){};

    //-------------------------------------------------------------------
    inline bool operator< ( const Intervalo& otroIntervalo ) const
    {
        return  ( L < otroIntervalo.L );
    }

    inline bool operator== ( const Intervalo& otroIntervalo ) const
    {
        return ( L == otroIntervalo.L ) && ( R == otroIntervalo.R );
    }

    inline bool operator<= ( const Intervalo& otroIntervalo ) const
    {
        return ( L <= otroIntervalo.L );
    }

    inline bool operator!= ( const Intervalo& otroIntervalo ) const
    {
        return !( *this == otroIntervalo );
    }

    inline const Intervalo& operator= ( const Intervalo& otroIntervalo )
    {
        this->L = otroIntervalo.L;
        this->R = otroIntervalo.R;

        return *this;
    }

    //-------------------------------------------------------------------
    int L;
    int R;
};
typedef std::vector< Intervalo >    ListaIntervalos;

ostream& operator<<( ostream& os, const Intervalo& intervalo )
{
    os << intervalo.L << " " << intervalo.R;
    return os;
}
//-------------------------------------------------------------------
// TestCase Class
//-------------------------------------------------------------------
class TestCase
{
public:
    //-------------------------------------------------------------------
    TestCase( int m, const ListaIntervalos& intervalos ):
        mM( m ), mIntervalos( intervalos ){};
    ~TestCase(){};

    //-------------------------------------------------------------------
    void resolver( ListaIntervalos& resultado )
    {
        ListaIntervalos intervalos( mIntervalos );
        sort( intervalos.begin(), intervalos.end() );

        resultado = cubrimientoMinimal( intervalos );
    }

private:
    //-------------------------------------------------------------------
    ListaIntervalos cubrimientoMinimal( ListaIntervalos& intervalos )
    {
        int intervaloCubierto = 0;
        int i = 0, size = intervalos.size();
        ListaIntervalos resultado;

        if( size != 0 )
        {
            while( i < size && intervaloCubierto < mM )
            {
                Intervalo maximoIntervalo = intervalos[ i ];
                while ( i < size && ( intervalos[ i ].L <= intervaloCubierto || intervalos[ i ].R <= intervaloCubierto ) )
                {
                    if( intervalos[ i ].R > maximoIntervalo.R )
                    {
                        maximoIntervalo = intervalos[ i ];
                    }
                    i++;
                }

                if( i < size && maximoIntervalo.L > intervaloCubierto )
                {
                    return ListaIntervalos();
                }

                resultado.push_back( maximoIntervalo );
                intervaloCubierto = maximoIntervalo.R;
            }
        }

        if ( intervaloCubierto < mM )
        {
            return ListaIntervalos();
        }

        return resultado;
    }
protected:
    //-------------------------------------------------------------------
    int mM;
    ListaIntervalos mIntervalos;
};

//-------------------------------------------------------------------
// Input y Output
//-------------------------------------------------------------------

void readInput( istream& is, std::vector< TestCase >& testCases )
{
    testCases.clear();

    int cantCasosDeTestRestantes;
    is >> cantCasosDeTestRestantes;

    Intervalo cero(0, 0);

    while( cantCasosDeTestRestantes > 0 )
    {
        int m;
        is >> m;

        ListaIntervalos intervalos;
        int L, R;
        is >> L;
        is >> R;
        Intervalo intervaloActual( L, R );

        while( intervaloActual != cero )
        {
            intervalos.push_back( intervaloActual );

            is >> intervaloActual.L;
            is >> intervaloActual.R;
        }

        testCases.push_back( TestCase( m, intervalos ) );
        cantCasosDeTestRestantes--;
    }
}

void writeOutput( ostream& os, const vector< ListaIntervalos >& resultados )
{
    for( size_t numTest = 0; numTest < resultados.size(); numTest++ )
    {
        const ListaIntervalos& resultadoTest( resultados[ numTest ] );

        int cantidadIntervalos = resultadoTest.size();
        os << cantidadIntervalos << endl;
        for( int i = 0; i < cantidadIntervalos; i++ )
        {
            os << resultadoTest[ i ] << endl;
        }

        os << endl;
    }
}

//-------------------------------------------------------------------
// Main function
//-------------------------------------------------------------------

int main()
{
    vector< TestCase > tests;

    readInput( cin, tests );

    vector< ListaIntervalos > resultados;
    for( size_t i = 0; i < tests.size(); i++ )
    {
        ListaIntervalos intervalosCubrimientoMin;

        tests[ i ].resolver( intervalosCubrimientoMin );
        resultados.push_back( intervalosCubrimientoMin );
    }

    writeOutput( cout, resultados );

    return 0;
}
