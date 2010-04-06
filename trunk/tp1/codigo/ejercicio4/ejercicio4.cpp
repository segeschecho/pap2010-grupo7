//-------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------

#include<vector>
#include<fstream>
#include<iostream>

using namespace std;

//-------------------------------------------------------------------
//Intervalo Class
//-------------------------------------------------------------------
class Intervalo
{
public:
    //-------------------------------------------------------------------
    Intervalo( int L, int R );
    ~Intervalo();

    //-------------------------------------------------------------------
    bool operator< ( const Intervalo& otroIntervalo ) const;
    bool operator== ( const Intervalo& otroIntervalo ) const;
    bool operator<= ( const Intervalo& otroIntervalo ) const;
    bool operator!= ( const Intervalo& otroIntervalo ) const;
    const Intervalo& operator= ( const Intervalo& otroIntervalo );

    //-------------------------------------------------------------------
    int L;
    int R;
};
typedef std::vector< Intervalo >    ListaIntervalos;

//-------------------------------------------------------------------
//Intervalo Class Methods

Intervalo::Intervalo( int L, int R )
{
    this->L = L;
    this->R = R;
}

Intervalo::~Intervalo()
{
}

bool Intervalo::operator<( const Intervalo& otroIntervalo ) const
{
    return  ( L < otroIntervalo.L );
}

bool Intervalo::operator==( const Intervalo& otroIntervalo ) const
{
    return ( L == otroIntervalo.L ) && ( R == otroIntervalo.R );
}

bool Intervalo::operator<=( const Intervalo& otroIntervalo ) const
{
    return ( L <= otroIntervalo.L );
}

const Intervalo& Intervalo::operator=( const Intervalo& otroIntervalo )
{
    this->L = otroIntervalo.L;
    this->R = otroIntervalo.R;

    return *this;
}

bool Intervalo::operator!=( const Intervalo& otroIntervalo ) const
{
    return !( *this == otroIntervalo );
}
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
    TestCase( int m, const ListaIntervalos& intervalos );
    ~TestCase();

    //-------------------------------------------------------------------
    int getM();
    const Intervalo& getIntervalo( int index );
    void resolver( ListaIntervalos& resultado );

private:
    //-------------------------------------------------------------------
    void quickSort( int desde, int hasta, ListaIntervalos& intervalos );
    int particionar( int desde, int hasta, ListaIntervalos& intervalos );
    ListaIntervalos cubrimientoMinimal( ListaIntervalos& intervalos );

protected:
    //-------------------------------------------------------------------
    int mM;
    ListaIntervalos mIntervalos;
};

//-------------------------------------------------------------------
// TestCase Class Methods

TestCase::TestCase( int m, const ListaIntervalos& intervalos ) :
    mM( m ), mIntervalos( intervalos )
{
}

TestCase::~TestCase()
{
}

int TestCase::getM()
{
    return mM;
}

const Intervalo& TestCase::getIntervalo( int index )
{
    return mIntervalos[ index ];
}

void TestCase::resolver( ListaIntervalos& resultado )
{
    ListaIntervalos intervalos( mIntervalos );
    quickSort( 0, intervalos.size() - 1, intervalos );

    resultado = cubrimientoMinimal( intervalos );
}


ListaIntervalos TestCase::cubrimientoMinimal( ListaIntervalos& intervalos )
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


int TestCase::particionar( int desde, int hasta, ListaIntervalos& intervalos )
{
    Intervalo pivot = intervalos[ desde ];
    int i = desde;
    int j = hasta;

    while( i < j )
    {
        while( i < j && pivot <= intervalos[ j ] )
        {
            j--;
        }

        while( i < j && intervalos[ i ] <= pivot )
        {
            i++;
        }

        if( i < j )
        {
            Intervalo temp = intervalos[ i ];
            intervalos[ i ] = intervalos[ j ];
            intervalos[ j ] = temp;
        }
    }

    Intervalo temp = intervalos[ i ];
    intervalos[ i ] = intervalos[ desde ];
    intervalos[ desde ] = temp;

    return i;
}
void TestCase::quickSort( int desde, int hasta, ListaIntervalos& intervalos )
{
    if ( desde < hasta )
    {
        int indicePivot = particionar( desde, hasta, intervalos );
        quickSort( desde, indicePivot - 1, intervalos );
        quickSort( indicePivot + 1, hasta, intervalos );
    }
}

//-------------------------------------------------------------------
// Application Class
//-------------------------------------------------------------------
class Application
{
public:
    //-------------------------------------------------------------------
    // constructores
    Application();
    ~Application();

    //-------------------------------------------------------------------
    // funcion principal
    void go( const char* fileName = NULL );

private:
    //-------------------------------------------------------------------
    void readInputFile( const char* fileName, std::vector< TestCase >& testCases );
    void writeOutputFile( const char* fileName, const std::vector< ListaIntervalos >& resultados );

    void readInput( istream& is, std::vector< TestCase >& testCases );
    void writeOutput( ostream& os, const std::vector< ListaIntervalos >& resultados );
};

//-------------------------------------------------------------------
// Application Class Methods

Application::Application()
{
}

Application::~Application()
{
}

void Application::go( const char* fileName )
{
    vector< TestCase > tests;

    if( fileName == NULL )
    {
        readInput( cin, tests );
    }
    else
    {
        readInputFile( fileName, tests );
    }

    vector< ListaIntervalos > resultados;
    for( size_t i = 0; i < tests.size(); i++ )
    {
        ListaIntervalos intervalosCubrimientoMin;

        tests[ i ].resolver( intervalosCubrimientoMin );
        resultados.push_back( intervalosCubrimientoMin );
    }

    if( fileName == NULL )
    {
        writeOutput( cout, resultados );
    }
    else
    {
        std::string fileOutputName( fileName );
        fileOutputName += "Out";
        writeOutputFile( fileOutputName.c_str(), resultados );
    }
}

void Application::readInput( istream& is, std::vector< TestCase >& testCases )
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

void Application::readInputFile( const char* fileName, std::vector< TestCase >& testCases )
{
    fstream archivo(fileName, fstream::in);

    if( archivo.fail() )
	{
        string expresion( "No se pudo abrir el archivo %s\n", fileName );
        throw expresion;
    }
    else
    {
        readInput( archivo, testCases );
    }

    archivo.close();
}

void Application::writeOutput( ostream& os, const vector< ListaIntervalos >& resultados )
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

void Application::writeOutputFile( const char* fileName, const vector< ListaIntervalos >& resultados )
{
    fstream archivo(fileName, fstream::out);

    if( archivo.fail() )
	{
        string expresion( "No se pudo escribir el archivo %s\n", fileName );
        throw expresion;
    }
    else
    {
        writeOutput( archivo, resultados );
    }

    archivo.close();
}

//-------------------------------------------------------------------
//Main function
//-------------------------------------------------------------------

int main(int argc, char* argv[])
{

    Application app;
    if ( argc > 1 )
    {
        for( int numArchivo = 1; numArchivo < argc; numArchivo++ )
        {
            app.go( argv[ numArchivo ] );
        }
    }
    else
    {
        app.go();
    }

    return 0;
}
