#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
using namespace std;

typedef std::vector< std::string > Vstring;
typedef std::vector< int > Vint;

#define CHAR_OFFSET_MIN 97
#define CHAR_OFFSET_MAX 123

bool letraYaUsada[ 16 ][ 26 ];

inline int cantMaxPotencialesES( int sizePalabra )
{
    // La cantidad maxima total de potenciales Edit Step de una palabra, siendo L su largo, es
    //        L,            que corresponde a sacar la i-esima letra
    // + 26 * L,            que corresponde a cambiar la i-esima letra por todas las letras del abecedario
    // + 26 * (L + 1),      que corresponde a agregar una letra del abecedario antes de la iesima posicion (0 < i <= L)
    // ----------------
    // 53L + 26

    return 56*sizePalabra + 26;
}

bool probablementeExiste( const std::string& palabra, int offsetPosicion )
{
    bool resultado = true;
    for( string::const_iterator it = palabra.begin(); it < palabra.end(); it++ )
    {
        resultado = resultado && letraYaUsada[ offsetPosicion ][ *it - CHAR_OFFSET_MIN ];
        offsetPosicion++;
    }

    return resultado;
}

int generarPalabras( const std::string& palabra, Vstring& resultado )
{
    int contadorPotencialesES = 0;

    int cantES = cantMaxPotencialesES( palabra.size() );
    resultado.clear();
    
    resultado.resize( cantES );

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

        if( probablementeExiste( parteIzq, 0 ) )
        {
            parteDer = palabra.substr( i + 1, largoPalabra );
            if( probablementeExiste( parteDer, i ) )
            {
                /* saco la letra i */
                temp = parteIzq;
                temp.append( parteDer );

                /* la agrego al resultado */
                resultado[ contadorPotencialesES++ ] = temp;
            }
            
            if( probablementeExiste( parteDer, i + 1 ) )
            {
                // modifico letras
                // se pueden filtrar letras si i == 0
                for( int ascii = CHAR_OFFSET_MIN; ascii < CHAR_OFFSET_MAX; ascii++ )
                {
                    if( letraYaUsada[ i ][ ascii - CHAR_OFFSET_MIN ] )
                    {
                        temp.clear();
                        temp = parteIzq;
                        temp.push_back( ascii );
                        temp.append( parteDer );

                        /* la agrego a res */
                        resultado[ contadorPotencialesES++ ] = temp;
                    }
                }
            }
        }
    }

    // agrego letras
    if ( palabra.size() < 16 )
    {
        if( probablementeExiste( palabra, 1 ) )
        {
            /*
             *  agrego letras al principio. Notar que ignoramos aquellos potenciales edit step
             *  que empiecen con letra > palabra[ 0 ], dado que el input viene ordenado,
             *  por lo tanto no existe ningun predecesor que empiece con letras > palabra[ 0 ]
             */

            char primeraLetra = palabra[ 0 ];
            for( int ascii = CHAR_OFFSET_MIN; ascii <= primeraLetra; ascii++ )
            {
                if( letraYaUsada[ 0 ][ ascii - CHAR_OFFSET_MIN ] )
                {
                    string temp;

                    /* agrego la letra al principio */
                    temp.push_back( ascii );
                    temp.append( palabra );

                    /* la agrego al resultado */
                    resultado[ contadorPotencialesES++ ] = temp;
                    temp.clear();
                }
            }
        }

        /* agrego desde la segunda letra*/
        for( int i = 1; i <= largoPalabra; i++ )
        {
            string parteIzq;
            string parteDer;

            parteIzq = palabra.substr( 0, i );
            if( probablementeExiste( parteIzq, 0 ) )
            {
                parteDer = palabra.substr( i, largoPalabra );
                if( probablementeExiste( parteDer, i + 1 ) )
                {
                    /* agrego todas las posibles letras en la posicion i */
                    for( int ascii = CHAR_OFFSET_MIN; ascii < CHAR_OFFSET_MAX; ascii++ )
                    {
                        if( letraYaUsada[ i ][ ascii - CHAR_OFFSET_MIN ] )
                        {
                            string temp;

                            temp = parteIzq;
                            temp.push_back( ascii );
                            temp.append( parteDer );

                            /* la agrego al resultado */
                            resultado[ contadorPotencialesES++ ] = temp;
                            temp.clear();
                        }
                    }
                }
            }
        }
    }

    return contadorPotencialesES;
}

int buscar( const string& palabra, const pair< string, int > palabras[], int desde, int hasta )
{
    while( desde < hasta )
    {
        int medio = (desde + hasta) / 2;

        // Si palabra es igual a la palabra del medio del arreglo
        // entonces ya la encontre
        if( palabra.compare( palabras[ medio ].first ) == 0 )
        {
            return medio;
        }

        // Si palabra es menor a la palabra del medio del arreglo
        // seguir buscando por la primer mitad del arreglo
        if( palabra.compare( palabras[ medio ].first ) == -1 )
        {
            hasta = medio - 1;
        }

        // Si palabra es mayor a la palabra del medio del arreglo
        // seguir buscando por la segunda mitad del arreglo
        if( palabra.compare( palabras[ medio ].first ) == 1 )
        {
            desde = medio + 1;
        }
    }

    if( palabra.compare( palabras[ desde ].first ) != 0 )
    {
        return -1;
    }

    return desde;
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
int maxEditStepLadder( pair< string, int > palabras[], int cantPalabras )
{
    int maxEscalera = 1;

    //  El vector posiblesESActual contiene los potenciales edit step de la palabra actual
    Vstring posiblesESActual;

    for( int i = 0; i < cantPalabras; i++ )
    {
        if( i > 0 )
        {
            // Calculo todas las potenciales palabras edit step de la actual
            int cantPosiblesES = generarPalabras( palabras[ i ].first, posiblesESActual );

            int maxEditStepLadderActual = 0;
            for( int j = 0; j < cantPosiblesES; j++ )
            {
                // busco cada potencial ES en el arreglo, pero solamente busco desde 0 a i - 1,
                // siendo i el indice de la palabra actual
                int indice = buscar( posiblesESActual[ j ], palabras, 0, i - 1 );
                if( indice >= 0 && maxEditStepLadderActual < palabras[ indice ].second )
                {
                    maxEditStepLadderActual = palabras[ indice ].second;
                }
            }

            palabras[ i ].second = maxEditStepLadderActual + 1;
            if( maxEscalera < palabras[ i ].second )
            {
                maxEscalera = palabras[ i ].second;
            }
        }

        // Habilito todas las letras de la palabra actual
        int posicion = 0;
        for( string::iterator it = palabras[ i ].first.begin(); it < palabras[ i ].first.end() && posicion < 16; it++ )
        {
            letraYaUsada[ posicion ][ *it - CHAR_OFFSET_MIN ] = true;
            posicion++;
        }
    }

    return maxEscalera;
}

int main()
{
    string s;
    pair< string, int > palabras[ 25000 ];

    int cantPalabras = 0;

/*
    fstream file( "test", ios_base::in );

    while ( !file.eof() )
    {
        getline( file, s );
        palabras[ cantPalabras++ ] = pair< string, int >( s, 1 );
    }

*/

    while(cin >> s)
    {
        //if (palabras.size() > 1)
        //    cout << palabras.back() << " con " << s << " es menor? " << palabras.back().compare(s) << endl;
        palabras[ cantPalabras++ ] = pair< string, int >( s, 1 );
    }

    cout << maxEditStepLadder( palabras, cantPalabras );


    return 0;
}
