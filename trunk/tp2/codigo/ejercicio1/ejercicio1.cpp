#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

typedef unsigned int uint;
typedef pair< uint, uint > Casillero;

typedef vector< bool > Vbool;
typedef vector< Vbool > Vvbool;
typedef vector< Vvbool > Vvvbool;
typedef vector< Vvvbool > Vvvvbool;

enum Direccion{ ARRIBA, DERECHA, ABAJO, IZQUIERDA };

class Nodo
{
public:
    Nodo( Casillero posCaja, Casillero posKeeper, uint pasos = 0, uint empujones = 0 ) : pC( posCaja ), pK( posKeeper ), p( pasos ), e( empujones ) {};
    Casillero pC;
    Casillero pK;
    uint p;
    uint e;
};

class TestCase
{
public:
    TestCase(){};
    ~TestCase(){};

    pair< int, int > resolver()
    {
        uint filas = hayPared.size();
        uint columnas = hayPared[ 0 ].size();

        Vvvvbool expandi( filas, Vvvbool( columnas, Vvbool( filas, Vbool( columnas, false ) ) ) );
        vector< Nodo > cola;

        cola.push_back( Nodo( mCasInicialCaja, mCasInicial ) );
        uint numNodoActual = 0;

        // mientras que aun hayan acciones por realizar y la caja no esta en el destino
        while( numNodoActual < cola.size() && cola[ numNodoActual ].pC != mGoal )
        {
            Nodo nodoActual = cola[ numNodoActual ];

            Casillero pC = nodoActual.pC;
            Casillero pK = nodoActual.pK;

            // si no expandi el nodo actual
            if( !expandi[ pC.first ][ pC.second ][ pK.first ][ pK.second ] )
            {
                for( Direccion d = ARRIBA; d <= IZQUIERDA; d = static_cast< Direccion >( d + 1 ) )
                {
                    Nodo nuevoNodo = moverme( nodoActual, d );
                    if( !hayPared[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ] && !hayPared[ nuevoNodo.pK.first ][ nuevoNodo.pK.second ] )
                        cola.push_back( moverme( nodoActual, d ) );
                }

                expandi[ pC.first ][ pC.second ][ pK.first ][ pK.second ] = true;
            }

            numNodoActual++;
        }

        if( numNodoActual == cola.size() )
        {
            return pair< int, int >( -1, -1 );
        }
        return pair< int, int >( cola[ numNodoActual ].p, cola[ numNodoActual ].e );
    }

    bool valido()
    {
        return mValido;
    }

    bool puedoMoverme( Nodo n , Direccion d )
    {
        Nodo nuevoNodo = moverme( n, d );
        if( !hayPared[ nuevoNodo.pC.first ][ nuevoNodo.pC.second ] && !hayPared[ nuevoNodo.pK.first ][ nuevoNodo.pK.second ] )
            return true;
        return false;
    }

    Nodo moverme( Nodo n , Direccion d )
    {
        uint filaK = n.pK.first, columnaK = n.pK.second, filaC = n.pC.first, columnaC = n.pC.second;
        uint nuevaFilaK = filaK, nuevaColK = columnaK, nuevaFilaC = filaC, nuevaColC = columnaC;
        uint empujo = 0;

        switch( d )
        {
        case IZQUIERDA:
            nuevaColK = columnaK - 1;
            // si la caja estaba a mi izquierda supongo que la caja se va a mover
            if( nuevaFilaK == filaC && nuevaColK == columnaC )
            {
                nuevaColC = columnaC - 1;
                empujo = 1;
            }
            break;

        case DERECHA:
            nuevaColK = columnaK + 1;
            // si la caja estaba a mi derecha supongo que la caja se va a mover
            if( nuevaFilaK == filaC && nuevaColK == columnaC )
            {
                nuevaColC = columnaC + 1;
                empujo = 1;
            }
            break;

        case ARRIBA:
            nuevaFilaK = filaK - 1;
            // si la caja estaba arriba mio supongo que la caja se va a mover
            if( nuevaFilaK == filaC && nuevaColK == columnaC )
            {
                nuevaFilaC = filaC - 1;
                empujo = 1;
            }
            break;

        case ABAJO:
            nuevaFilaK = filaK + 1;
            // si la caja estaba abajo mio supongo que la caja se va a mover
            if( nuevaFilaK == filaC && nuevaColK == columnaC )
            {
                nuevaFilaC = filaC + 1;
                empujo = 1;
            }
            break;
        };

        return Nodo( Casillero( nuevaFilaC, nuevaColC ), Casillero( nuevaFilaK, nuevaColK ), n.p + 1, n.e + empujo );
    }

    // casillero inicial de la caja
    Casillero mCasInicialCaja;

    // destino donde llegar la caja
    Casillero mGoal;

    // casillero inicial del cuidador del almacén (wharehouse keeper)
    Casillero mCasInicial;

    // representativo de las paredes del almacén
    vector< vector< bool > > hayPared;

    bool mValido;
};

istream& operator>>( istream& is, TestCase& t )
{
    uint filas, columnas;
    if( is >> filas )
    {
        is >> columnas;
        if( filas == 0 || columnas == 0 )
        {
            t.mValido = false;
            return is;
        }

        // ignoro el caracter de '\n' que hay luego del k
        is.ignore();
        // FIXME: ARREGLAR ESTO EN EL MOMENTO DE LA ENTREGA PARA QUE SEA GETCHAR
        char c = is.peek();
        is.ignore();

        // el filas + 2 y columnas + 2 es para agregarle una pared alrededor
        t.hayPared = vector< vector< bool > > ( filas + 2, vector< bool >( columnas + 2, true ) );

        // parseamos filas x columnas caracteres
        for( uint fila = 1; fila < filas + 1; fila++ )
        {
            for( uint col = 1; col < columnas + 1; col++ )
            {
                if( c == '\n' )
                {
                    c = is.peek();
                    is.ignore();
                }

                switch( c )
                {
                case '.':
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'T':
                    t.mGoal = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'B':
                    t.mCasInicialCaja = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
                    break;

                case 'S':
                    t.mCasInicial = Casillero( fila, col );
                    t.hayPared[ fila ][ col ] = false;
                    break;

                default:
                    break;
                };

                c = is.peek();
                is.ignore();
            }
        }
    }

    return is;
}

#define FILEINPUT

int main()
{
#ifdef FILEINPUT

    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );

    int numTest = 1;
    bool testValido = true;
    while( testValido )
    {
        TestCase testActual;
        entrada >> testActual;
        testValido = testActual.valido();
        if( testValido )
        {
            pair< int, int > res = testActual.resolver();

            salida << "Instancia " << numTest << endl;
            if( res.first >= 0 )
                salida << res.first << " " << res.second << endl << endl;
            else
                salida << "Impossivel" << endl << endl;

            numTest++;
        }
    }

    salida.close();
    entrada.close();

#else

    int numTest = 1;
    bool testValido = true;
    while( testValido )
    {
        TestCase testActual;
        cin >> testActual;
        testValido = testActual.valido();
        if( testValido )
        {
            pair< int, int > res = testActual.resolver();

            cout << "Instancia " << numTest << endl;
            if( res.first >= 0 )
                cout << res.first << " " << res.second << endl << endl;
            else
                cout << "Impossivel" << endl << endl;

            numTest++;
        }
    }

#endif
    return 0;
}
