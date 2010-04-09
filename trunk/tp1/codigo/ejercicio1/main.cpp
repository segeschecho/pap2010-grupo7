#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
using namespace std;

typedef std::vector< std::string > Vstring;
typedef std::vector< int > Vint;
typedef std::map< string, int > MapStringInt;

void generarPalabras( const std::string& palabra, Vstring* resultado[] )
{
    /***
    a-z esta entre los ascii 97-122 inclusive
    ***/
    int largoPalabra = palabra.size();

    /* saco de a una letra por vez y lo agrego al resultado */
    /* en el mismo ciclo modifico letras de la palabra (saco y agrego letras)y */
    for (int i = 0; i < largoPalabra; i++)
    {
        string temp;
        string parteIzq;
        string parteDer;

        /* inicio las var */
        parteIzq = palabra.substr( 0, i );
        parteDer = palabra.substr( i + 1, largoPalabra );

        /* saco la letra i */
        temp = parteIzq;
        temp.append( parteDer );

        /* la agrego al resultado */
        resultado[ 0 ]->push_back( temp );

        ///modifico letras
        /// se pueden filtrar letras si i == 0
        for( int ascii = 97; ascii < 123; ascii++ )
        {
            temp.clear();
            temp = parteIzq;
            temp.push_back( ascii );
            temp.append( parteDer );

            /* la agrego a res */
            resultado[ 1 ]->push_back(temp);
        }
    }

    ///agrego letras
    /* agrego letras al principio y filtro las que no van */
    char primeraLetra = palabra[ 0 ];
    for( int ascii = 97; ascii <= primeraLetra; ascii++ )
    {
        string temp;

        /* agrego la letra al principio */
        temp.push_back( ascii );
        temp.append( palabra );

        /* la agrego al resultado */
        resultado[ 2 ]->push_back( temp );
        temp.clear();
    }

    /* agrego desde la segunda letra*/
    for( int i = 1; i <= largoPalabra; i++ )
    {
        /* agrego todas las posibles letras en la posicion i */
        for( int ascii = 97; ascii < 123; ascii++ )
        {
            string temp;
            string parteIzq;
            string parteDer;

            /* inicio las var */
            parteIzq = palabra.substr( 0, i );
            parteDer = palabra.substr( i, largoPalabra );

            temp = parteIzq;
            temp.push_back( ascii );
            temp.append( parteDer );

            /* la agrego al resultado */
            resultado[ 2 ]->push_back( temp );
            temp.clear();
        }
    }
}

/*
 *  algoritmo maxEditStepLadder:
 *      1: inicializar maximo escalera en 0
 *      2: para cada palabra hacemos:
 *          3: generar sus posibles edit ladders
 *          4: por cada una de estos potenciales edit step, vemos si existe una palabra igual
 *             a este edit step. Lo hacemos buscando en un diccionario ( O(log(n)) )
 *             el maximo edit step ladder para este potencial edit step.
 *          5: me quedo con el mayor step ladder de todos los potenciales edit step,
 *             y asigno mayor step ladder + 1 en el diccionario de los max edit step ladder
 *             para la palabra actual
 *          6: actualizo maxima escalera segun el step ladder encontrado para la palabra actual
 *      5: fin.
 */
int maxEditStepLadder( const Vstring& palabras )
{
    int maximaEscalera = 0;

    MapStringInt* largoMaxEditStepLadder[ 16 ];
    for( int i = 0; i < 16; i++ )
    {
        largoMaxEditStepLadder[ i ] = new std::map< string, int >();
    }

    /*  El vector posiblesEditStepPalabraActual contiene los potenciales edit step de una palabra
     *  Sea L el largo de la palabra en cuestion:
     *  el indice 0 contiene los posibles edit step de largo L - 1
     *  el indice 1 contiene los posibles edit step de largo L
     *  el indice 2 contiene los posibles edit step de largo L + 1
     */
    Vstring* posiblesEditStepPalabraActual[ 3 ];
    for( int i = 0; i < 3; i++ )
    {
        posiblesEditStepPalabraActual[ i ] = new Vstring();
    }

    size_t cantPalabras = palabras.size();
    for( size_t indPActual = 0; indPActual < cantPalabras; indPActual++ )
    {
        // Calculo todas las potenciales palabras edit step de la actual
        generarPalabras( palabras[ indPActual ], posiblesEditStepPalabraActual );

        // Primero recorro las potenciales palabras edit step de menor longitud
        // luego las de igual longitud, y luego las de mayor.
        int maxEditStepLadderActual = 0;
        for( int j = 0; j < 3; j++ )
        {
            size_t cantPosiblesES = posiblesEditStepPalabraActual[ j ]->size();

            int sizeESActual;
            if( cantPosiblesES > 0 )
            {
                sizeESActual = (*posiblesEditStepPalabraActual[ j ])[ 0 ].size();
                if ( largoMaxEditStepLadder[ sizeESActual - 1 ]->size() > 0 )
                {
                    for( size_t k = 0; k < cantPosiblesES; k++ )
                    {
                        string& potencialEditStep = ( *posiblesEditStepPalabraActual[ j ] )[ k ];
                        MapStringInt::const_iterator it = largoMaxEditStepLadder[ sizeESActual - 1 ]->find( potencialEditStep );
                        if( it != largoMaxEditStepLadder[ sizeESActual - 1 ]->end() )
                        {
                            if( maxEditStepLadderActual < it->second )
                            {
                                maxEditStepLadderActual = it->second;
                            }
                        }
                    }
                }
            }
        }

        std::pair< string, int > lMESLPalabraActual;
        lMESLPalabraActual.first = palabras[ indPActual ];
        lMESLPalabraActual.second = maxEditStepLadderActual + 1;

        if( maximaEscalera < lMESLPalabraActual.second )
        {
            maximaEscalera = lMESLPalabraActual.second;
        }

        largoMaxEditStepLadder[ palabras[ indPActual ].size() - 1 ]->insert( lMESLPalabraActual );

        for( int j = 0; j < 3; j++ )
        {
            posiblesEditStepPalabraActual[ j ]->clear();
        }
    }


    for( int i = 0; i < 3; i++ )
    {
        delete posiblesEditStepPalabraActual[ i ];
    }

    for( int i = 0; i < 16; i++ )
    {
        delete largoMaxEditStepLadder[ i ];
    }

    return maximaEscalera;
}

int main()
{
    string s;
    Vstring palabras;

    while ( !cin.eof() )
    {
        getline( cin, s );
        palabras.push_back( s );
    }

    cout << maxEditStepLadder(palabras);

    return 0;
}