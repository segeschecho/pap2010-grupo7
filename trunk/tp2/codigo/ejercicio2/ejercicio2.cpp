#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/*
 *  Estructuras generales
 */
#define horaReloj pair<int, int>
#define INF horaReloj(1000000, 0)
 //eje = (hora salida, tiempo viaje, siguiente nodo)
#define eje pair < pair <horaReloj, horaReloj>, int >
 // datos nodo = (hora llegada, mejor tiempo viaje, tocado?)
#define datosNodo pair <pair <horaReloj, horaReloj>, bool >
 // salidaYViaje = (hora salida, tiempo viaje)
#define salidaYViaje pair < horaReloj, horaReloj >

/*
 * macros
 */
#define definirEje(p, s, v, sig) p.first.first = s; p.first.second = v; p.second = sig

/*
 * Definiciones de funciones auxiliares
 */
inline void obtenerHora(istream& is, horaReloj& hora);
inline void sumarConLimite(horaReloj& h1, horaReloj& h2);
inline void sumarSinLimite(horaReloj& h1, horaReloj& h2);
inline void mostrarTiempoViaje(ostream& os, horaReloj& h);
inline void mostrarHora(ostream& os, horaReloj& h);
inline void diferencia (horaReloj& h1, horaReloj& h2, horaReloj &res);
inline void restarHora(horaReloj& h1, horaReloj& h2, horaReloj& res);
inline bool compararDatosNodo(datosNodo &d1, datosNodo& d2);



class TestCase{
    friend istream& operator>>(istream& is, TestCase& t){

        int cantLineas;
        int cantEstaciones;
        int numNodoAsignar = 0;
        int numNodoAnterior;
        string ciudad;
        horaReloj horaSalida;

        is >> cantLineas;

        for(int linea = 0; linea < cantLineas; linea++){
            is >> cantEstaciones;

            for(int estacion = 0; estacion < cantEstaciones; estacion++){
                horaReloj viaje;
                int nodoCiudad;
                bool nodoNuevo = false;
                eje e;

                obtenerHora(is, viaje);
                is >> ciudad;

                //veo si la estacion ya aparecio
                if(t.estaCiudad(ciudad)){
                    nodoCiudad = t.ciudadANodo[ciudad];
                }
                else{
                    nodoNuevo = true;
                    t.ciudadANodo[ciudad] = numNodoAsignar;
                    t.adyacentesDe.push_back(vector< eje >());
                    t.tiempos.push_back(datosNodo(pair<horaReloj, horaReloj>(INF,INF), false));

                    nodoCiudad = numNodoAsignar;
                }

                if(estacion == 0){
                    //inicio hora de salida
                    horaSalida = viaje;
                    numNodoAnterior = nodoCiudad;
                    if(nodoNuevo)
                        numNodoAsignar++;
                }
                else{
                    //armo el eje con los datos
                    definirEje(e, horaSalida, viaje, nodoCiudad);

                    //agrego la info al nodo anterior
                    t.adyacentesDe[numNodoAnterior].push_back(e);

                    sumarConLimite(horaSalida, viaje);
                    numNodoAnterior = nodoCiudad;

                    if(nodoNuevo)
                        numNodoAsignar++;
                }
            }
        }

        //guardo el origen y destino
        string origenStr;
        string destinoStr;

        is >> origenStr;
        is >> destinoStr;

        t.origen = t.ciudadANodo[origenStr];
        t.destino = t.ciudadANodo[destinoStr];

        ///muestro a ver si levanta las cosas
/*        for (int i = 0; i < t.adyacentesDe.size(); i++){
            for(int j = 0; j < t.adyacentesDe[i].size(); j++){
                cout << "adyacente de: " << i;
                cout << " es " << t.adyacentesDe[i][j].second;
                cout << " salida ";
                mostrarHora(cout, t.adyacentesDe[i][j].first.first);
                cout << " viaje ";
                mostrarTiempoViaje(cout, t.adyacentesDe[i][j].first.second);
                cout << endl;
            }
        }

        for (int i = 0; i < t.adyacentesDe.size(); i++){
            cout << "tiempos de " << i << " hora ";
            mostrarHora(cout, t.tiempos[i].first.first);
            cout << " viaje ";
            mostrarHora(cout, t.tiempos[i].first.second);
            cout << " tocado " << t.tiempos[i].second << endl;
        }

        cout << "origen " << t.origen;
        cout << " destino " << t.destino << endl;
*/
        return is;

    }

public:
        TestCase(){}

        void resolver(){
            int cantAdyOrigen;

            cantAdyOrigen = adyacentesDe[origen].size();

            for(int adyOrigen = 0; adyOrigen < cantAdyOrigen; adyOrigen++){
                //para cada uno de los adyacentes al origen calculo el mejor tiempo
                //de viaje al destino

                eje ejeActual = adyacentesDe[origen][adyOrigen];
                int nodoActual = ejeActual.second;
                horaReloj horaLlegada = ejeActual.first.first;

                //marco el nodo actual
                tiempos[nodoActual].second = true;

                sumarConLimite(horaLlegada, ejeActual.first.second); //salida + viaje
                tiempos[nodoActual].first.first = horaLlegada;
                tiempos[nodoActual].first.second = ejeActual.first.second;

                while (nodoActual != destino){
                    int cantAdyActual = adyacentesDe[nodoActual].size();
                    for(int ady = 0; ady < cantAdyActual; ady++){
                        ejeActual = adyacentesDe[nodoActual][ady];
                        int nodoAdy = ejeActual.second;
                        horaReloj horaSalidaAdy = ejeActual.first.first;
                        horaReloj viajeAdy = ejeActual.first.second;
                        horaReloj espera;
                        horaReloj horaLlegadaNueva;
                        horaReloj tiempoViajeNuevo;

                        diferencia(horaLlegada, horaSalidaAdy, espera);

                        horaLlegadaNueva = horaLlegada;
                        sumarConLimite(horaLlegadaNueva, espera);
                        sumarConLimite(horaLlegadaNueva, viajeAdy);

                        tiempoViajeNuevo = tiempos[nodoActual].first.second;
                        sumarSinLimite(tiempoViajeNuevo, espera);
                        sumarSinLimite(tiempoViajeNuevo, viajeAdy);

                        if(tiempoViajeNuevo < tiempos[nodoAdy].first.second){
                            //actualizo el mejor tiempo para el nodo ady
                            tiempos[nodoAdy].first.first = horaLlegadaNueva;
                            tiempos[nodoAdy].first.second = tiempoViajeNuevo;
                        }
                    }

                    //busco el proximo nodo
                    tiempos[nodoActual].second = true;
                    nodoActual = buscarProximoNodo();

                    if(nodoActual == -1)
                        break;

                    horaLlegada = tiempos[nodoActual].first.first;
                }
                ///acordarse de marcar el destino
                tiempos[destino].second = true;

                //guardo el resultado
                horaReloj tempHoraSalida = adyacentesDe[origen][adyOrigen].first.first;
                horaReloj tempTiempoViaje = tiempos[destino].first.second;
                resultados.push_back(salidaYViaje(tempHoraSalida, tempTiempoViaje));

                //desmarco todos los nodos de nuevo
                for(int i = 0; i < tiempos.size(); i++){
                    tiempos[i].first.first = INF;
                    tiempos[i].first.second = INF;
                    tiempos[i].second = false;
                }
            }

            //filtro resultados
            vector < salidaYViaje > resultadosFiltrados;
            filtrarResultados(resultadosFiltrados);

            ///muestro resultados
            for(int i = resultadosFiltrados.size() - 1; i >= 0 ; i--){
                mostrarHora(cout, resultadosFiltrados[i].first);
                cout << " ";
                mostrarTiempoViaje(cout, resultadosFiltrados[i].second);
                cout << endl;
            }
        }
private:

    /*
     *  Variables
     */
    map <string, int> ciudadANodo;
    vector < vector < eje > > adyacentesDe;
    vector < datosNodo > tiempos;
    vector < salidaYViaje > resultados;
    int origen;
    int destino;


    /*
     *  Metodos
     */

    inline bool estaCiudad(string ciudad){
        map<string ,int> :: iterator it;

        it = ciudadANodo.find(ciudad);
        if(it != ciudadANodo.end()){
            return true;
        }else{
            return false;
        }
    }

    inline int buscarProximoNodo(){
        horaReloj min = INF;
        int nodoRes = -1;

        for(int i = 0; i < tiempos.size(); i++){
            if(tiempos[i].first.second < min && !tiempos[i].second){
                min = tiempos[i].first.second;
                nodoRes = i;
            }
        }
        return nodoRes;
    }

    inline void filtrarResultados(vector < salidaYViaje >& res){
        horaReloj minLlegadaDiaSig = INF;
        vector < salidaYViaje > tempRes;

        sort(resultados.begin(), resultados.end());

        //saco los que tienen misma hora salida
        for(int i = 0; i < resultados.size(); i++){
            if(tempRes.size() == 0){
                tempRes.push_back(resultados[i]);
            }

            if(tempRes[tempRes.size() - 1].first != resultados[i].first){
                tempRes.push_back(resultados[i]);
            }
        }

        //busco el que minimiza salida + viaje + 24hs
        for(int i = 0; i < tempRes.size(); i++){
            horaReloj temp(24, 0);
            sumarSinLimite(temp, tempRes[i].first);
            sumarSinLimite(temp, tempRes[i].second);

            if(temp < minLlegadaDiaSig)
                minLlegadaDiaSig = temp;
        }

        //recorro de atras para adelante
        for(int i = tempRes.size() - 1; i >= 0; i--){
            horaReloj temp;
            temp = tempRes[i].first;
            sumarSinLimite(temp, tempRes[i].second);

            if (temp < minLlegadaDiaSig){
                minLlegadaDiaSig = temp;
                res.push_back(tempRes[i]);
            }
        }
    }

};


int main(){
    int cantCasos;

    cin >> cantCasos;

    while (cantCasos > 0){
        TestCase test;

        cin >> test;

        test.resolver();
        if(cantCasos != 1)
            cout << endl;

        cantCasos--;
    }

    return 0;
}

/*
 * Funciones auxiliares
 */

//toma la hora y la covierte en horaReloj
inline void obtenerHora(istream& is, horaReloj& hora){
    string horaString;

    //obtengo la hora del trayecto
    is >> horaString;

    //la convierto en horaReloj
    int pos = horaString.find(":");
    hora.first = atoi(horaString.substr(0, pos).c_str());
    hora.second = atoi(horaString.substr(pos + 1, 2).c_str());
}

//suma horas, pero manteniendose en el rango usual
inline void sumarConLimite(horaReloj& h1, horaReloj& h2){
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

//suma horas, pero no hace modulo 24hs
inline void sumarSinLimite(horaReloj& h1, horaReloj& h2){
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
}

// muestra la hora de la forma hhh...h:mm
inline void mostrarTiempoViaje(ostream& os, horaReloj& h){

    os << h.first << ":";
    if(h.second < 10)
        os << "0" << h.second;
    else
        os << h.second;
}

//muestra la hora de la forma hh:mm
inline void mostrarHora(ostream& os, horaReloj& h){
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

//calcula la diferencia entre 2 horas mod 24
inline void diferencia (horaReloj& h1, horaReloj& h2, horaReloj &res){

    if(h1 == h2){
        res = horaReloj(0,0);
        return;
    }

    if (h1 < h2){
        restarHora(h2, h1, res);
    }
    else{
        horaReloj dif;
        horaReloj h24(24, 0);

        restarHora(h1, h2, dif);
        restarHora(h24, dif, res);

    }
}

//resta horas suponiendo h1 mas grande que h2
inline void restarHora(horaReloj& h1, horaReloj& h2, horaReloj& res){
    int minutos = 0;
    int horas = 0;

    if(h2.second > h1.second){
        minutos = 60 + (h1.second - h2.second);
        horas -= 1;
    }
    else
        minutos = h1.second - h2.second;

    horas = horas + h1.first - h2.first;
    res = horaReloj(horas, minutos);
}

inline bool compararDatosNodo(const salidaYViaje &d1, const salidaYViaje& d2){
    return (d1.first < d2.first);
}
