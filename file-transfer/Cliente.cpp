#include <iostream>
#include <fstream>
#include <sstream>
#include "Solicitud.h"

/*
Variables foráneas:
  ->LIMITE_ARCHIVO
  ->Ubicación: Mensaje.h

Entidades foráneas:
  ->struct mensaje
  ->Ubicación: Mensaje.h

  ->Solicitud
  ->Ubicación: Solicitud.h
*/

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "--!! ERROR: Mala invocación del programa. Intente:" << endl;
    cout << "./cliente direccion_ip_del_servidor nombre_del_archivo" << endl;
    return 0;
  }
  else if (strlen(argv[1])>16) {
    cout << "--!! ERROR: IP inválida. Inténtelo de nuevo." << endl;
    return 0;
  }

  Solicitud s;
  struct mensaje mensajeEnvio;
  struct mensaje mensajeRecibo;
  ifstream archivoLeer;
  ifstream archivoEnviar;
  char* nombreLeer;
  string linea;
  int tamArchivo;
  int tamString;
  string convertir;
  stringstream strs;

  archivoLeer.open(argv[2]);

  if (archivoLeer.is_open()) {
    while (getline(archivoLeer,linea)) {
      nombreLeer = new char[linea.length()+1];
      strcpy(nombreLeer, linea.c_str());
      archivoEnviar.open(nombreLeer);

      if (archivoEnviar.is_open()) {
        archivoEnviar.seekg(0, ios::end);
        tamArchivo = archivoEnviar.tellg();
        strs << tamArchivo;
        convertir = strs.str();
        strs.str(std::string());

        for (int c = 0; c < (int)(convertir.length()); c++) {
          mensajeEnvio.tam[c] = (convertir.c_str())[c];
        }
        
        mensajeEnvio.tam[(int)(convertir.length())] = '\0';

        if (tamArchivo > LIMITE_ARCHIVO) {
          cout << "==================================" << endl;
          cout << "--!! ADVERTENCIA: " << nombreLeer << " pesa "
               << mensajeEnvio.tam << " bytes y sobrepasa el límite de "
               << LIMITE_ARCHIVO << " bytes." << endl;
          cout << "El archivo será omitido." << endl;
        }
        else {
          for (int k = 0; k < linea.length()+1; k++)
            mensajeEnvio.nombreArchivo[k] = nombreLeer[k];

          mensajeEnvio.nombreArchivo[linea.length()+1] = '\0';
          mensajeEnvio.estatus = 0;
          archivoEnviar.seekg(0, ios::beg);
          archivoEnviar.read(mensajeEnvio.archivo, tamArchivo);
          memcpy(
              (char*)&mensajeRecibo,
              s.doOperation(argv[1], 7200, (char*)&mensajeEnvio),
              sizeof(struct mensaje));
          cout << "==================================" << endl;
          cout << mensajeRecibo.nombreArchivo << ": ";

          if (mensajeRecibo.estatus == '0') {
            cout << "Recibido." << endl;
          }
          else {
            cout << "Error al recibir." << endl;
          }
        }

        archivoEnviar.close();
      }
      else {
        cout << "==================================" << endl;
        cout << "--!! ADVERTENCIA: " << nombreLeer
             << " es un archivo inexistente." << endl;
        cout << "El archivo será omitido." << endl;
      }

      delete[] nombreLeer;
    }

    archivoLeer.close();
  }
  else {
    cout << "--!! ERROR: " << argv[2] << " es un archivo inexistente." << endl;
  }

  return 0;
}
