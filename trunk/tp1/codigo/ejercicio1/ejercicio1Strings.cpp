#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_PALABRAS 25000
#define MAX_SIZE_PALABRA 16

string palabras[ MAX_PALABRAS ];
int maxLadders[ MAX_PALABRAS ];

int buscar( const string& palabra, int desde, int hasta )
{
    while( desde < hasta )
    {
        int medio = (desde + hasta) / 2;

        int comparacion = palabra.compare( palabras[ medio ] );
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

    if( palabra.compare( palabras[ desde ] ) != 0 )
    {
        return -1;
    }

    return desde;
}

int maxEditStepLadderPalabra( int indiceActual )
{
    string& palabra = palabras[ indiceActual ];
    int longPalabra = palabra.size();

    int maxLadderActual = 1;

    string temp = palabra;

    // elimino letras
    // saco de a una letra por vez y lo agrego al resultado

    // si el largo de la palabra es 1 no tenemos que eliminar letras

    if( longPalabra > 1 )
    {
        for( int i = 0; i < longPalabra; i++ )
        {
            char iesimaLetra = palabra[ i ];
            if( i == longPalabra - 1 || temp[ i + 1 ] <= iesimaLetra )
            {
                temp.erase( i, 1 );

                int indiceES = buscar( temp, 0, indiceActual );
                if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
                {
                    maxLadderActual = maxLadders[ indiceES ] + 1;
                }

                temp.insert( i, 1, iesimaLetra );
            }
        }
    }

    // cambio letras
    // aprovechamos que el input viene ordenado, y no buscamos palabras que
    // sean de mayor orden lexicografico
    string::iterator letraOriginal = palabra.begin();
    for( string::iterator it = temp.begin(); it < temp.end(); it++ )
    {
        for( int ascii = 'a'; ascii < *letraOriginal; ascii++ )
        {
            *it = ascii;
            int indiceES = buscar( temp, 0, indiceActual );
            if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
            {
                maxLadderActual = maxLadders[ indiceES ] + 1;
            }
        }

        *it = *letraOriginal;
        letraOriginal++;
    }

    // agrego letras
    // aprovechamos que el input viene ordenado, y no buscamos palabras que
    // sean de mayor orden lexicografico

    // ademas, no buscamos palabras de longitud >= 16, entonces:
    if( longPalabra < MAX_SIZE_PALABRA )
    {
        temp.insert( 0, 1, 'a' );
        // si bien estamos agregando una letra a la palabra, queremos recorrerla con
        // i < longPalabra, porque si i = longPalabra significa que vamos a agregar una
        // letra al final de la palabra, lo que resulta en una palabra de orden lexicografico
        // mayor, y eso no tiene sentido, ya que el input viene ordenado y podemos aprovechar
        // esto ya que sabemos que no habra ningun predecesor que sea igual a la palabra
        // analizada pero con una letra mas al final.

        string::iterator letraOriginal = palabra.begin();
        for( string::iterator it = temp.begin(); it < temp.end() && letraOriginal < palabra.end(); it++ )
        {
            for( int ascii = 'a'; ascii < *letraOriginal; ascii++ )
            {
                *it = ascii;
                int indiceES = buscar( temp, 0, indiceActual );
                if( indiceES >= 0 && maxLadders[ indiceES ] + 1 > maxLadderActual )
                {
                    maxLadderActual = maxLadders[ indiceES ] + 1;
                }
            }
            *it = *letraOriginal;
            letraOriginal++;
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

inline int maxEditStepLadder( int cantPalabras )
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
    ifstream file ( "test7", ios_base::in );

    string s;
    while ( file >> s )
    {
        palabras[ cantPalabras ] = s;
        maxLadders[ cantPalabras ] = 1;
        cantPalabras++;
    }
*/
    string s;
    while( cin >> s )
    {
        palabras[ cantPalabras ] = s;
        maxLadders[ cantPalabras ] = 1;
        cantPalabras++;
    }

    if( cantPalabras == 0 )
    {
        cout << "0";
    }
    else
    {
        cout << maxEditStepLadder( cantPalabras );
    }

    return 0;
}
