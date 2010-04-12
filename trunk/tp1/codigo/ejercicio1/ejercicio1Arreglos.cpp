#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_PALABRAS 25000
#define MAX_SIZE_PALABRA 16

char palabras[ MAX_PALABRAS ][ MAX_SIZE_PALABRA + 1 ];
int longPalabras[ MAX_PALABRAS ];
int maxLadders[ MAX_PALABRAS ];

inline bool isLower( char c )
{
    return 'a' <= c && c <= 'z';
}

int buscar( const char palabra[], int desde, int hasta )
{
    while( desde < hasta )
    {
        int medio = (desde + hasta) / 2;

        int comparacion = strcmp( palabra, palabras[ medio ] );
        // Si palabra es igual a la palabra del medio del arreglo
        // entonces ya la encontre
        if( comparacion == 0 )
        {
            return medio;
        }

        // Si palabra es menor a la palabra del medio del arreglo
        // seguir buscando por la primer mitad del arreglo
        if( comparacion < 0 )
        {
            hasta = medio - 1;
        }

        // Si palabra es mayor a la palabra del medio del arreglo
        // seguir buscando por la segunda mitad del arreglo
        if( comparacion > 0 )
        {
            desde = medio + 1;
        }
    }

    if( strcmp( palabra, palabras[ desde ] ) != 0 )
    {
        return -1;
    }

    return desde;
}

inline void borrarLetra( int index, char palabra[], int longPalabra )
{
    for( int i = index; i < longPalabra; i++ )
    {
        palabra[ i ] = palabra[ i + 1 ];
    }
}

inline void correrALaDerecha( int index, char palabra[], int longPalabra )
{
    for( int i = longPalabra; i >= index; --i )
    {
        palabra[ i + 1 ] = palabra[ i ];
    }
}

inline void agregarLetra( int index, char palabra[], char letra, int longPalabra )
{
    correrALaDerecha( index, palabra, longPalabra );

    palabra[ index ] = letra;
}

int maxEditStepLadderPalabra( int indiceActual )
{
    char* palabra = palabras[ indiceActual ];
    int longPalabra = longPalabras[ indiceActual ];

    int maxLadderActual = 1;

    char temp[ MAX_SIZE_PALABRA + 1 ];
    strcpy( temp, palabra );

    // elimino letras
    // saco de a una letra por vez y lo agrego al resultado

    // si el largo de la palabra es 1 no tenemos que eliminar letras

    if( longPalabra > 1 )
    {
        for( int i = 0; i < longPalabra; i++ )
        {
            if( i == longPalabra - 1 || temp[ i + 1 ] <= palabra[ i ] )
            {
                borrarLetra( i, temp, longPalabra );

                int indiceES = buscar( temp, 0, indiceActual );
                if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
                {
                    maxLadderActual = maxLadders[ indiceES ] + 1;
                }

                agregarLetra( i, temp, palabra[ i ], longPalabra - 1 );
            }
        }
    }

    // cambio letras
    // aprovechamos que el input viene ordenado, y no buscamos palabras que
    // sean de mayor orden lexicografico
    for( int i = 0; i < longPalabra; i++ )
    {
        for( int ascii = 'a'; ascii < palabra[ i ]; ascii++ )
        {
            temp[ i ] = ascii;
            int indiceES = buscar( temp, 0, indiceActual );
            if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
            {
                maxLadderActual = maxLadders[ indiceES ] + 1;
            }
        }

        temp[ i ] = palabra[ i ];
    }

    // agrego letras
    // aprovechamos que el input viene ordenado, y no buscamos palabras que
    // sean de mayor orden lexicografico

    // ademas, no buscamos palabras de longitud >= 16, entonces:
    if( longPalabra < MAX_SIZE_PALABRA )
    {
        correrALaDerecha( 0, temp, longPalabra );
        // si bien estamos agregando una letra a la palabra, queremos recorrerla con
        // i < longPalabra, porque si i = longPalabra significa que vamos a agregar una
        // letra al final de la palabra, lo que resulta en una palabra de orden lexicografico
        // mayor, y eso no tiene sentido, ya que el input viene ordenado y podemos aprovechar
        // esto ya que sabemos que no habra ningun predecesor que sea igual a la palabra
        // analizada pero con una letra mas al final.

        for( int i = 0; i < longPalabra; i++ )
        {
            for( int ascii = 'a'; ascii < palabra[ i ]; ascii++ )
            {
                temp[ i ] = ascii;
                int indiceES = buscar( temp, 0, indiceActual );
                if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
                {
                    maxLadderActual = maxLadders[ indiceES ] + 1;
                }
            }
            temp[ i ] = temp[ i + 1 ];
        }
    }

    return maxLadderActual;
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

inline int maxEditStepLadder( int maxLadders[], int cantPalabras )
{
    int maxEscalera = 1;

    for( int i = 1; i < cantPalabras; i++ )
    {
        maxLadders[ i ] = maxEditStepLadderPalabra( i );

        if( maxEscalera < maxLadders[ i ] )
        {
            maxEscalera = maxLadders[ i ];
        }
    }
    return maxEscalera;
}

int main()
{
    int cantPalabras = 0;
/*
    ifstream file ( "test2", ios_base::in );

    string s;
    while ( file >> s )
    {
        strcpy( palabras[ cantPalabras ], s.c_str() );
        longPalabras[ cantPalabras ] = s.size();
        maxLadders[ cantPalabras ] = 1;
        cantPalabras++;
    }
*/
    string s;
    while( cin >> s )
    {
        strcpy( palabras[ cantPalabras ], s.c_str() );
        longPalabras[ cantPalabras ] = s.size();
        maxLadders[ cantPalabras ] = 1;
        cantPalabras++;
    }

    if( cantPalabras == 0 )
    {
        cout << "0";
    }
    else
    {
        cout << maxEditStepLadder( maxLadders, cantPalabras );
    }

    return 0;
}
