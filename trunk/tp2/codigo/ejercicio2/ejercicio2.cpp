#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define horaReloj pair< int, int >
#define INF pair< int, int >(24, 0)


//-----------------------------------------------------------------------------------
// Operadores hora reloj
//-----------------------------------------------------------------------------------

void operator +=(horaReloj& h1, horaReloj& h2){
    //sumo los minutos
    h1.second += h2.second;
    //me fijo si se paso de 60
    if (h1.second > 60){
        int temp = h1.second;
        h1.second = temp % 60;
        h1.first += temp / 60;
    }

    //sumo las horas
    h1.first += h2.first;
    //me fijo si se paso de 24
    if(h1.first > 24){
        int temp = h1.first;
        h1.first = temp % 24;
    }
}

bool operator <(horaReloj& h1, horaReloj& h2) {
    if(h1.first == h2.first)
        return (h1.second < h2.second);
    else
        return (h1.first < h2.first);
}

ostream& operator<<(ostream& os, horaReloj& h){
    os << h.first << ":" << h.second;
}

//-----------------------------------------------------------------------------------
// Clase Eje
//-----------------------------------------------------------------------------------
/*
 *  Esta clase representa un eje con los datos <origen, destino, tiempoViaje, horarioSalida>
 */
class Eje
{
	public:
        Eje();

		Eje(int origen, int destino, horaReloj tiempoViaje, horaReloj horarioSalida){
		    this->o = origen;
		    this->d = destino;
		    this->tViaje = tiempoViaje;
		    this->hSalida = horarioSalida;
		}

		int origen(){
            return o;
		}

        int destino(){
            return d;
        }

        horaReloj tiempoViaje(){
            return tViaje;
        }

        horaReloj horarioSalida(){
            return hSalida;
        }

		~Eje(){}

	private:

	int o;
	int d;
	horaReloj tViaje;
	horaReloj hSalida;

};

class TestCase{
    friend istream& operator>>(istream& is, TestCase& t){
        int rutas;

        is >> rutas;

        //contador que asignara un numero a cada ciudad nueva
        int numeroNodo = 0;

        for(int ruta = 0; ruta < rutas; ruta++){
            //horario representa la hora en que va a pasar el tren en cada estacion
            horaReloj horario;
            horario.first = 0;
            horario.second = 0;

            //tengo la estacion anterior a la actual
            int estacionAnterior;

            //cantidad de estaciones de la ruta actual
            int estaciones;
            is >> estaciones;

            cout << "estaciones: " << estaciones << endl;

            //primero guardo la estacion inicial del recorrido
            if(estaciones > 0){
                string ciudad;
                horaReloj hora;
                //cargo la hora en formato horaReloj
                t.obtenerHora(is, hora);

                //actualizo el horario
                horario = hora;
                cout << "sale: " << hora.first << ":" << hora.second;
                cout << " horario: " << horario.first << ":" << horario.second;

                is >> ciudad;
                cout << " " << ciudad << endl;

                //cargo la info del nodo inicial
                //me fijo si la cuidad ya esta
                map<string ,int> :: iterator it;
                it = t.ciudadNodo.find(ciudad);

                if(it != t.ciudadNodo.end()){
                    //si ya esta el nodo entonces me guardo el nodo y el horario
                    int nodo = it->second;

                    estacionAnterior = nodo;
                }
                else{
                    //sino esta, agrego el nodo a las estructuras
                    t.tiempos.push_back(pair <horaReloj, bool>(INF, false));
                    //le asigno un numero a la ciudad
                    t.ciudadNodo[ciudad] = numeroNodo;
                    //agrego la hora del tren que pasa por la ciudad
                    t.listaAdy.push_back(vector < Eje >() );
                    estacionAnterior = numeroNodo;
                    //aumento el indice para la sig nueva ciudad
                    numeroNodo++;
                }
            }

            for(int estacion = 1; estacion < estaciones; estacion++){
                //recorro las estaciones y voy guardando a informacion
                string ciudad;
                horaReloj hora;

                //cargo la hora en el formato horaReloj
                t.obtenerHora(is, hora);

                //obtengo la ciudad
                is >> ciudad;
                cout << " " << ciudad << endl;

                //guardo la informacion y voy armando el grafo
                //me fijo si la ciudad ya esta
                map<string ,int> :: iterator it;
                it = t.ciudadNodo.find(ciudad);

                if(it != t.ciudadNodo.end()){
                    //si esta actualizo la info
                    int nodo = it->second;

                    //creo el eje que representa el tramo
                    Eje e(estacionAnterior, nodo, hora, horario);

                    //lo agrego al nodo de la estacion anterior a la actual
                    t.listaAdy[estacionAnterior].push_back(e);
                }
                else{
                    //sino esta, agrego el nodo a las estructuras
                    t.tiempos.push_back(pair<horaReloj, bool>(INF, false));
                    //le asigno un numero a la ciudad
                    t.ciudadNodo[ciudad] = numeroNodo;

                    Eje e(estacionAnterior, numeroNodo, hora, horario);

                    //agrego la info en la lista de adyacencias
                    t.listaAdy[estacionAnterior].push_back(e);
                    //genero el espacio para el proximo nodo
                    t.listaAdy.push_back(vector < Eje >() );
                    //aumento el indice para la sig nueva ciudad
                    numeroNodo++;
                }

                //calculo el horario de salida para el proximo nodo
                horario += hora;

                //agrego a la ciudad anterior la estacion actual
                it = t.ciudadNodo.find(ciudad);
                estacionAnterior = it->second;
            }

        }

        //guardo la ciudad origen y destino
        string origen;
        string destino;

        is >> origen;
        is >> destino;

        t.origen = t.ciudadNodo[origen];
        t.destino = t.ciudadNodo[destino];

        //muestro para ver que anda
        for (int i = 0; i < numeroNodo; i++){
            //muestro los adyacentes
            cout << "adyacentes de: " << i;
            for (int j = 0; j < t.listaAdy[i].size(); j++){
                cout << " " << t.listaAdy[i][j].origen() << " es: "
                << t.listaAdy[i][j].destino() << " con tiempo:"
                << t.listaAdy[i][j].tiempoViaje().first << ":"
                << t.listaAdy[i][j].tiempoViaje().second
                << " horario Partida: " << t.listaAdy[i][j].horarioSalida().first << ":"
                << t.listaAdy[i][j].horarioSalida().second <<endl;
            }
            cout << endl;
        }
        cout << endl;

        cout << "ciudad origen: " << origen << "numero: " << t.origen << endl;
        cout << "ciudad destino: " << destino << "numero: " << t.destino << endl;

        for (int i = 0; i < numeroNodo; i++){
            cout << t.tiempos[i].first.first << " " << t.tiempos[i].first.second << endl;
        }


        return is;
    }

public:
    TestCase(){};

    void resolver(){
        //hacemos dijkstra para cada uno de las ciudades vecinas al origen
        int adyacentesOrigen = listaAdy[origen].size();

        //marcamos el origen como recorrido
        tiempos[origen].second = true;

        for(int adyOrigen = 0; adyOrigen < adyacentesOrigen; adyOrigen++){
            //nodo con el que comienzo dijktra
            int nodoActual = listaAdy[origen][adyOrigen].destino();
            int adyacentesActual = listaAdy[nodoActual].size();

            //menor tiempo hasta el momento
            horaReloj tiempoTotal;

            //nodo al que se pasara en la siguiente iteracion
            int nodoMenor;
            int posNodoMenor;
            horaReloj tiempoMenor;

            tiempoTotal = listaAdy[origen][adyOrigen].tiempoViaje();

            while(adyacentesActual > 0 and nodoActual != destino){
                //inicializo el tiempo menor
                tiempoMenor = INF;

                for(int adyActual = 0; adyActual < adyacentesActual; adyActual++){
                    Eje ejeAdyActual = listaAdy[nodoActual][adyActual];
                    int nodoAdy = ejeAdyActual.destino();

                    if(!tiempos[nodoAdy].second){
                        //si el nodo no se recorrio
                        horaReloj temp = ejeAdyActual.tiempoViaje();
                        temp += tiempoTotal;

                        if(temp < tiempos[nodoAdy].first){
                            //si es menor la distancia, entonces actualizo la dist
                            tiempos[nodoAdy].first = temp;
                        }

                        //me fijo si es el siguiente nodo
                        if(tiempos[nodoAdy].first < tiempoMenor){
                            tiempoMenor = tiempos[nodoAdy].first;
                            nodoMenor = nodoAdy;
                            posNodoMenor = nodoAdy;
                        }
                    }
                }

                //actualizo el tiempo total
                horaReloj temp = listaAdy[nodoActual][posNodoMenor].tiempoViaje();
                tiempoTotal += temp;
                //indico cual es el siguiente nodo
                nodoActual = nodoMenor;
                adyacentesActual = listaAdy[nodoActual].size();
            }

            horaReloj temp = listaAdy[origen][adyOrigen].horarioSalida();
            cout << temp << " ";
            cout << "tiempo: " << tiempoTotal << endl;
        }
    }

    ~TestCase(){};
private:
    //------------------------------------------------------------------------------
    void obtenerHora(istream& is, horaReloj& hora){
        string horaString;

        //obtengo la hora del trayecto
        is >> horaString;

        //la convierto en horaReloj
        int pos = horaString.find(":");
        hora.first = atoi(horaString.substr(0, pos).c_str());
        hora.second = atoi(horaString.substr(pos + 1, 2).c_str());
    }
    //------------------------------------------------------------------------------
    // mapeo entre nombre de cuidad y numero de nodo
    map<string, int> ciudadNodo;

    //lista de adyacencia para los nodos, con el tiempo que tarda de ir de uno a otro
    vector < vector < Eje > > listaAdy;

    //ciudad origen y destino
    int origen;
    int destino;

    //arreglo que tendra los tiempos minimos y si esta marcado
    vector < pair< horaReloj, bool > > tiempos;

};

int main()
{
    int cantCasos;

    cin >> cantCasos;

    while (cantCasos > 0){
        TestCase test;

        cout << "cargando test" << endl;
        cin >> test;

        //viene resolver
        test.resolver();

        cantCasos--;
    }

    return 0;
}
