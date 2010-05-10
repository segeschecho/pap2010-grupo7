#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#define horaReloj pair< int, int >
#define INF pair< int, int >(1000000, 0)


//-----------------------------------------------------------------------------------
// Operador para pair< pair<horaReloj, horaReloj>, pair< horaReloj, bool > >
//-----------------------------------------------------------------------------------
#define parFeo pair< pair<horaReloj, horaReloj>, pair< horaReloj, bool > >
bool operator <(const parFeo& p1, const parFeo& p2){
    return ( (p1.first.first < p2.first.first) && p1.second.second && p2.second.second);
}

//-----------------------------------------------------------------------------------
// Operadores hora reloj
//-----------------------------------------------------------------------------------

void sumarHorarios(horaReloj& h1, horaReloj& h2){
    //sumo los minutos
    h1.second += h2.second;
    //me fijo si se paso de 60
    if (h1.second >= 60){
        int temp = h1.second;
        h1.second = temp % 60;
        h1.first += temp / 60;
    }

    //sumo las horas
    h1.first += h2.first;
    //me fijo si se paso de 24
    if(h1.first >= 24){
        int temp = h1.first;
        h1.first = temp % 24;
    }
}

bool operator ==(horaReloj& h1, horaReloj& h2){
    return (h1.first == h2.first)&&(h1.second == h2.second);
}

void operator +=(horaReloj& h1, horaReloj& h2){
    //sumo los minutos
    h1.second += h2.second;
    //me fijo si se paso de 60
    if (h1.second >= 60){
        int temp = h1.second;
        h1.second = temp % 60;
        h1.first += temp / 60;
    }

    //sumo las horas
    h1.first += h2.first;
    //me fijo si se paso de 24
/*    if(h1.first >= 24){
        int temp = h1.first;
        h1.first = temp % 24;
    }*/
}

//h1 mas grande que h2
horaReloj& operator -(horaReloj& h1, horaReloj& h2) {
    int minutos = 0;
    int horas = 0;
    horaReloj res;

    if(h2.second > h1.second){
        minutos = 60 + (h1.second - h2.second);
        horas -= 1;
    }
    else
        minutos = h1.second - h2.second;

    horas = horas + h1.first - h2.first;
    res = pair<int, int>(horas, minutos);

    return res;
}

bool operator <(horaReloj& h1, horaReloj& h2) {
    if(h1.first == h2.first)
        return (h1.second < h2.second);
    else
        return (h1.first < h2.first);
}

bool operator <=(horaReloj& h1, horaReloj& h2) {
    if(h1.first == h2.first)
        return (h1.second <= h2.second);
    else
        return (h1.first < h2.first);
}

ostream& operator<<(ostream& os, horaReloj& h){

    os << h.first << ":";
    if(h.second < 10)
        os << "0" << h.second;
    else
        os << h.second;

    return os;
}

void mostrarHorario(ostream& os, horaReloj& h){
    if(h.first < 10)
        os << "0" << h.first;
    else
        os << h.first;

    os << ":";

    if(h.second < 10)
        os << "0" << h.second;
    else
        os << h.second;
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

        horaReloj& tiempoViaje(){
            return tViaje;
        }

        horaReloj& horarioSalida(){
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
            horario = horaReloj(0,0);

            //tengo la estacion anterior a la actual
            int estacionAnterior;

            //cantidad de estaciones de la ruta actual
            int estaciones;
            is >> estaciones;

            //cout << "estaciones: " << estaciones << endl;

            //primero guardo la estacion inicial del recorrido
            if(estaciones > 0){
                string ciudad;
                horaReloj hora;
                //cargo la hora en formato horaReloj
                t.obtenerHora(is, hora);

                if(hora < t.horarioSalidaSoloUnNodo){
                    t.horarioSalidaSoloUnNodo = hora;
                }

                //actualizo el horario
                horario = hora;
                //cout << "sale: " << hora;
                //cout << " horario: " << horario;

                is >> ciudad;
                //cout << " " << ciudad << endl;

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
                    t.horariosLlegada.push_back(horaReloj());
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
                //cout << " " << ciudad << endl;

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
                    t.horariosLlegada.push_back(horaReloj());
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
                sumarHorarios(horario, hora);

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
/*        for (int i = 0; i < numeroNodo; i++){
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
*/

        return is;
    }

public:
    TestCase(){
        horarioSalidaSoloUnNodo = horaReloj(24, 0);
    }

    void resolver(){
        //hacemos dijkstra para cada uno de las ciudades vecinas al origen
        int adyacentesOrigen = listaAdy[origen].size();

        //marcamos el origen como recorrido
        tiempos[origen].second = true;

        if(destino == origen){
            horaReloj min = listaAdy[origen][0].horarioSalida();

            if(adyacentesOrigen > 0){
                for(int i = 0; i < adyacentesOrigen; i++){
                    if(listaAdy[origen][i].horarioSalida() < min)
                        min = listaAdy[origen][i].horarioSalida();
                }
                mostrarHorario(cout, min);
                cout << " " << "0:00" << endl;
            }
            else{
                mostrarHorario(cout, horarioSalidaSoloUnNodo);
                cout << " " << "0:00" << endl;
            }
            return;
        }

        for(int adyOrigen = 0; adyOrigen < adyacentesOrigen; adyOrigen++){
            vector <int> tocados;
            //nodo con el que comienzo dijktra
            int nodoActual = listaAdy[origen][adyOrigen].destino();
            int adyacentesActual = listaAdy[nodoActual].size();

            bool hayMas = true;

            //menor tiempo hasta el momento
            horaReloj tiempoTotal;

            tiempoTotal = listaAdy[origen][adyOrigen].tiempoViaje();

            tiempos[nodoActual].first = tiempoTotal;
            tiempos[nodoActual].second = true;

            horariosLlegada[nodoActual] = listaAdy[origen][adyOrigen].horarioSalida();
            horariosLlegada[nodoActual] += tiempoTotal;

            tocados.push_back(nodoActual);

            while(hayMas){
                for(int adyActual = 0; adyActual < adyacentesActual; adyActual++){
                    Eje ejeAdyActual = listaAdy[nodoActual][adyActual];
                    int nodoAdy = ejeAdyActual.destino();

                    if(!tiempos[nodoAdy].second){
                        //si el nodo no se recorrio
                        horaReloj dif;

                        diferencia(horariosLlegada[nodoActual], ejeAdyActual.horarioSalida(), dif);
                        //cout << "diferencia: " << horariosLlegada[nodoActual] << " - " << ejeAdyActual.horarioSalida() << " es "<< dif << endl;
                        dif += ejeAdyActual.tiempoViaje();

                        dif += tiempos[nodoActual].first;

                        //cout << "dif :" << dif << endl;

                        if(dif < tiempos[nodoAdy].first){
                            //si es menor el tiempo para llegar, entonces actualizo
                            tiempos[nodoAdy].first = dif;
                            //tiempos[nodoAdy].first += tiempos[nodoActual].first;

                            //cout << "sumado: " << dif << endl;// << " + " << tiempos[nodoActual].first << endl;



                            horariosLlegada[nodoAdy] = ejeAdyActual.horarioSalida();
                            //cout << "horario llegada: " << horariosLlegada[nodoAdy] << " + " << ejeAdyActual.tiempoViaje() << endl;
                            sumarHorarios(horariosLlegada[nodoAdy], ejeAdyActual.tiempoViaje());

                        }
                        tocados.push_back(nodoAdy);
                    }
                }

                //indico cual es el siguiente nodo
                nodoActual = buscarMinimoNoTachado();

                if(nodoActual == -1){
                    hayMas = false;
                    break;
                }

                //actualizo el tiempo total
                tiempoTotal = tiempos[nodoActual].first;

                adyacentesActual = listaAdy[nodoActual].size();
/*                cout << "tiempo total: " << tiempoTotal << endl;
                cout << "nodo: " << nodoActual << endl;
                cout << "adyacentes: " << adyacentesActual << endl;
*/
                //marco el nodo como recorrido
                tiempos[nodoActual].second = true;
            }

            //actualizo los resultados
            horaReloj salida = listaAdy[origen][adyOrigen].horarioSalida();
            //cout << salida << " ";
            //cout << "tiempo: " << tiempos[destino].first << endl;
            horaReloj viaje = tiempos[destino].first;
            horaReloj llegada = horariosLlegada[destino];
            bool va = true;

            //me fijo si este intervalo, es contenido por otros, si es asi a los demas los saco
            for(int i = 0; i < resultados.size(); i++){
                //cout << "1 temp salida: " << tempSalida << " templlegada: " << tempLlegada << endl;
                //cout << "1 salida: " << salida << " llegada: " << llegada << endl;
                if(resultados[i].second.second){
                    horaReloj tempSalida = resultados[i].first.first;
                    horaReloj tempLlegada = resultados[i].first.second;
                    horaReloj tempViaje = resultados[i].second.first;

                    horaReloj difTempSalidaTempLlegada;
                    horaReloj difTempSalidaSalida;
                    horaReloj difTempSalidaLlegada;

                    diferencia(tempSalida, tempLlegada, difTempSalidaTempLlegada);
                    diferencia(tempSalida, salida, difTempSalidaSalida);
                    diferencia(tempSalida, llegada, difTempSalidaLlegada);

                    if(difTempSalidaSalida <= difTempSalidaTempLlegada && difTempSalidaLlegada <= difTempSalidaTempLlegada){
                        //|| (tempSalida == tempLlegada)){
                        //el nuevo intervalo esta contenido, entonces el que ya estaba se va
                        if (!(salida == llegada)){
                            resultados[i].second.second = false;
                        }

                    }
                    else{
                        //me fijo si el intervalo nuevo es cubierto
                        if(difTempSalidaTempLlegada < difTempSalidaSalida && difTempSalidaTempLlegada < difTempSalidaLlegada){
                            //si los 2 estan afuera
                            horaReloj difSalidaTempSalida;
                            horaReloj difSalidaTempLlegada;
                            horaReloj difSalidaLlegada;

                            diferencia(salida, tempSalida, difSalidaTempSalida);
                            diferencia(salida, tempLlegada, difSalidaTempLlegada);
                            diferencia(salida, llegada, difSalidaLlegada);

                            if(difSalidaTempSalida < difSalidaLlegada && difSalidaTempLlegada < difSalidaLlegada){
                                va = false;
                            }
                            else{
                                horaReloj max;
                                if(difSalidaTempSalida < difSalidaTempLlegada)
                                    max = difSalidaTempLlegada;
                                else
                                    max = difSalidaTempSalida;

                                if(max <= viaje)
                                    va = false;
                            }
                        }
                        else{
                            //quiere decir que alguna hora esta fuera del intervalo
                            horaReloj max;
                            if(difTempSalidaSalida < difTempSalidaLlegada)
                                max = difTempSalidaLlegada;
                            else
                                max = difTempSalidaSalida;

                            if(max <= tempViaje)
                                resultados[i].second.second = false;
                        }
                    }
                }
            }

            if(va){
                pair<horaReloj, horaReloj> a(salida, llegada);
                pair <horaReloj, bool> b(viaje, true);
                pair< pair< horaReloj, horaReloj >, pair< horaReloj, bool > > c(a, b);

                resultados.push_back(c);
            }

            //desmarco los nodos tocados
            for(int i = 0; i < (int)tocados.size(); i++){
                int nodo = tocados[i];
                tiempos[nodo].first = INF;
                tiempos[nodo].second = false;
            }
        }

        //muestro resultados ordenados por horario de salida
        sort(resultados.begin(), resultados.end());
        for (int i = 0; i < resultados.size(); i++){
            if (resultados[i].second.second){
                mostrarHorario(cout, resultados[i].first.first);
                cout << " " << resultados[i].second.first << endl;
            }
        }
    }

    ~TestCase(){};
private:
    //------------------------------------------------------------------------------
    void diferencia (horaReloj& h1, horaReloj& h2, horaReloj &res){

        if((h1.first == h2.first) && (h1.second == h2.second)){
            res = horaReloj(0,0);
            return;
        }

        if (h1 < h2){
            res = h2 - h1;
        }
        else{
            horaReloj hora24 = horaReloj(24,0);
            res = hora24 - h1;
            res += h2;
        }
    }
    //------------------------------------------------------------------------------
    int buscarMinimoNoTachado(){
        int res = -1;
        horaReloj min = INF;

        for(int i = 0; i < (int)tiempos.size(); i++){
            if(tiempos[i].first < min && !tiempos[i].second){
                min = tiempos[i].first;
                res = i;
            }
        }
        return res;
    }

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
    //horario de llegada a cada nodo
    vector< horaReloj > horariosLlegada;
    //resultados <salida, llegada, viaje, va>
    vector < pair< pair<horaReloj, horaReloj>, pair< horaReloj, bool > > > resultados;
    //para el caso en que hay solo un nodo
    horaReloj horarioSalidaSoloUnNodo;

};

int main()
{
    int cantCasos;

    cin >> cantCasos;

    while (cantCasos > 0){
        TestCase test;

        //cout << "cargando test" << endl;
        cin >> test;

        //viene resolver
        test.resolver();
        if(cantCasos != 1)
            cout << endl;

        cantCasos--;
    }

    return 0;
}
