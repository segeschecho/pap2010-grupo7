//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------

#include<iostream>
#include<sstream>
#include<vector>

using namespace std;

typedef std::pair< std::string, std::string > TestCase;

#define MAX_DIGITOS_COMPONENTE_VLUI 9
#define MAX_VALOR_COMPONENTE_VLUI 1000000000

//-------------------------------------------------------------------
// Class VeryLongUnsignedInt
//-------------------------------------------------------------------

class VeryLongUnsignedInt
{
    friend ostream& operator<< ( ostream& os, const VeryLongUnsignedInt& vlui);
public:
    VeryLongUnsignedInt()
    {
        mEnteros = vector< unsigned long long int >();
    }

    ~VeryLongUnsignedInt(){};

    VeryLongUnsignedInt operator+(const VeryLongUnsignedInt& otroVLUI) const
    {
        VeryLongUnsignedInt resultado;
        resultado = 0;
        resultado += *this;
        resultado += otroVLUI;

        return resultado;
    }

    VeryLongUnsignedInt operator+( unsigned long long int sumando ) const
    {
        VeryLongUnsignedInt resultado;

        resultado = sumando;
        resultado += *this;

        return resultado;
    }

    void operator=( const VeryLongUnsignedInt& otroVLUI )
    {
        mEnteros = otroVLUI.mEnteros;
    }

    void operator=( unsigned long long int entero )
    {
        if( mEnteros.size() == 0 )
        {
            if( entero >= MAX_VALOR_COMPONENTE_VLUI )
            {
                mEnteros.push_back( 0 );
                mEnteros.push_back( 0 );
                mEnteros[ 1 ] += entero / MAX_VALOR_COMPONENTE_VLUI;
                mEnteros[ 0 ] = entero % MAX_VALOR_COMPONENTE_VLUI;
            }
            else
            {
                mEnteros.push_back( entero );
            }
        }
        else
        {
            mEnteros[ 0 ] = entero;
            for( size_t i = 1; i < mEnteros.size(); i++ )
            {
                mEnteros[ i ] = 0;
            }
        }
    }

    void operator+=(unsigned long long int entero)
    {
        VeryLongUnsignedInt sumando;
        sumando = entero;
        *this += sumando;
    }

    void operator+=(const VeryLongUnsignedInt& otroVLUI)
    {
        for( size_t i = 0; i <= mEnteros.size() && i < otroVLUI.mEnteros.size(); i++ )
        {
            if( i == mEnteros.size() )
            {
                mEnteros.push_back( 0 );
            }

            mEnteros[ i ] += otroVLUI.mEnteros[ i ];

            // si hay carry
            if( mEnteros[ i ] >= MAX_VALOR_COMPONENTE_VLUI )
            {
                // si hay carry hasta mas alla del tamaño del VLUI, tengo que agregar un nuevo
                // entero para poder tener en cuenta el carry
                if( i == mEnteros.size() - 1 )
                {
                    mEnteros.push_back( 0 );
                }
                mEnteros[ i + 1 ] += mEnteros[ i ] / MAX_VALOR_COMPONENTE_VLUI;
                mEnteros[ i ] = mEnteros[ i ] % MAX_VALOR_COMPONENTE_VLUI;
            }
        }
    }

private:

    vector< unsigned long long int > mEnteros;
};

ostream& operator<< ( ostream& os, const VeryLongUnsignedInt& vlui)
{
    int ultimoEntero = vlui.mEnteros.size() - 1;
    for( int i = ultimoEntero; i >= 0; i-- )
    {
        os << vlui.mEnteros[ i ];
        if( i != ultimoEntero )
        {
            unsigned long long int num = vlui.mEnteros[ i ];
            int digitos = 1;
            while( num != 0 )
            {
                num = num / 10;
                digitos++;
            }

            while( digitos < MAX_DIGITOS_COMPONENTE_VLUI )
            {
                os << "0";
                digitos++;
            }
        }
    }

    return os;
}

//-------------------------------------------------------------------
// Lectura y escritura STD/Archivo
//-------------------------------------------------------------------

void writeOutput( ostream& os, const vector< VeryLongUnsignedInt >& resultadosTests )
{
    size_t size = resultadosTests.size();

    for( size_t i = 0; i < size; i++ )
    {
        os << resultadosTests[ i ] << endl;
    }
}


void readInput( istream& is, vector< TestCase >& testCases )
{
    testCases.clear();

    int cantCasosDeTestRestantes;
    is >> cantCasosDeTestRestantes;

    is.ignore();    // ignoro el fin de linea

    while( cantCasosDeTestRestantes > 0 )
    {
        TestCase test;

        getline( is, test.first );
        getline( is, test.second );

        testCases.push_back( test );

        cantCasosDeTestRestantes--;
    }
}

//-------------------------------------------------------------------
// Solucion al problema
//-------------------------------------------------------------------

VeryLongUnsignedInt resolver( const TestCase& test )
{
     vector< VeryLongUnsignedInt > combinaciones(test.second.size());

     for( size_t i = 0; i < combinaciones.size(); i++ )
     {
         combinaciones[ i ] = 0;
     }

     for( string::const_reverse_iterator itSec = test.first.rbegin(); itSec < test.first.rend(); itSec++ )
     {
         string::const_iterator itSubsec = test.second.begin();
         for( size_t i = 0; i < combinaciones.size() && itSubsec < test.second.end(); i++ )
         {
             if( *itSec == *itSubsec )
             {
                 // si es el ultimo tengo que sumar 1 solamente
                 if( i == combinaciones.size() - 1 )
                 {
                     combinaciones[ i ] += 1;
                 }
                 else
                 {
                     combinaciones[ i ] += combinaciones[ i + 1 ];
                 }
             }

             itSubsec++;
         }
     }

     return combinaciones[ 0 ];
}

int main()
{
    vector< TestCase > tests;

    readInput( cin, tests );

    vector< VeryLongUnsignedInt > resultados;
    size_t size = tests.size();
    for( size_t i = 0; i < size; i++ )
    {
        VeryLongUnsignedInt resultadoTestActual;

        resultadoTestActual = resolver( tests[ i ] );
        resultados.push_back( resultadoTestActual );
    }

    writeOutput( cout, resultados );

    return 0;

}
