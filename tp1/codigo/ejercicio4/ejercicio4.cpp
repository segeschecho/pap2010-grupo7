//-------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------

#include<vector>
#include<iostream>
#include<algorithm>

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

        int intervaloCubierto = 0;
        int i = 0, cantIntervalos = intervalos.size();

        if( cantIntervalos > 0 )
        {
            // Ignoro aquellos intervalos que no cubran nada del [0,infininto), ya que 
            // el intervalo a cubrir es [0, M]
            while( i < cantIntervalos && intervalos[ i ].R < 0 )
            {
                i++;
            }
            
            while( i < cantIntervalos && intervaloCubierto <= mM )
            {
                Intervalo& maximoIntervalo = intervalos[ i ];
                while ( i < cantIntervalos &&  intervalos[ i ].L <= intervaloCubierto )
                {
                    if( intervalos[ i ].R > maximoIntervalo.R )
                    {
                        maximoIntervalo = intervalos[ i ];
                    }
                    i++;
                }

                if( maximoIntervalo.L > intervaloCubierto )
                {
                    break;
                }

                resultado.push_back( maximoIntervalo );
                intervaloCubierto = maximoIntervalo.R;
            }
        }

        if ( intervaloCubierto < mM )
        {
            resultado.clear();
        }
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
