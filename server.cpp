#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <time.h>
#include <windows.h>
#include <winsock2.h>
#define PORT 5000

using namespace std;

class Server{
public:

    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];

    Server()
    {
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);

        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);

        cout << "Esperando conexion..." << endl << endl;
        char primerTextoLog[100];
        char puerto[6];
        memset(primerTextoLog, 0, sizeof(primerTextoLog));
        memset(puerto, 0, sizeof(puerto));
        strcpy(primerTextoLog, "Servidor esperando conexion. Puerto de escucha (");
        sprintf(puerto, "%d", PORT);
        strcat(primerTextoLog, puerto);
        strcat(primerTextoLog, ")");
        this->agregarTextoLog(primerTextoLog); // dd/mm/aaaa: Servidor esperando conexion. Puerto de escucha(PORT)
        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            cout << "Un cliente se ha conectado" << endl<< endl;
            this->agregarTextoLog("Cliente conectado");
        }
    }

    // Retorna dia actual del sistema [1;31]
    int getCurrentDayOfMonth()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_mday;
    }

    // Retorna mes actual del sistema [1;12]
    int getCurrentMonthValue()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_mon + 1;
    }

    // Retorna año actual del sistema (se le suma 1900 porque tm_year comienza en ese número)
    int getCurrentYear()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_year + 1900;
    }

    // Retorna hora actual del sistema [0;23]
    int getCurrentHour()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_hour;
    }

    // Retorna minuto actual del sistema [0;59]
    int getCurrentMinute()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_min;
    }

    // Retorna segundo actual del sistema [0;59]
    int getCurrentSecond()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_sec;
    }

    // Guarda en fechaYHoraActual[] la fecha y hora del sistema
    void getCurrentDateAndHour(char fechaYHoraActual[])
    {
        memset(fechaYHoraActual, 0, sizeof(fechaYHoraActual));
        char aux[3];
        memset(aux, 0, sizeof(aux));
        strcpy(fechaYHoraActual, "[");
        sprintf(aux, "%d", this->getCurrentDayOfMonth());
        strcat(fechaYHoraActual, aux);
        strcat(fechaYHoraActual, "/");
        memset(aux, 0, sizeof(aux));
        sprintf(aux, "%d", this->getCurrentMonthValue());
        strcat(fechaYHoraActual, aux);
        strcat(fechaYHoraActual, "/");
        memset(aux, 0, sizeof(aux));
        sprintf(aux, "%d", this->getCurrentYear());
        strcat(fechaYHoraActual, aux);
        strcat(fechaYHoraActual, " - ");
        memset(aux, 0, sizeof(aux));
        if(this->getCurrentHour() < 10)
        {
            strcat(fechaYHoraActual, "0");
        }
        sprintf(aux, "%d", this->getCurrentHour());
        strcat(fechaYHoraActual, aux);
        strcat(fechaYHoraActual, ":");
        memset(aux, 0, sizeof(aux));
        if(this->getCurrentMinute() < 10)
        {
            strcat(fechaYHoraActual, "0");
        }
        sprintf(aux, "%d", this->getCurrentMinute());
        strcat(fechaYHoraActual, aux);
        strcat(fechaYHoraActual, "]");
        memset(aux, 0, sizeof(aux));
    }

    // 1º Abre el archivo en modo lectura
    // 2º Resetea el buffer
    // 3º Le agrega el texto "Muestra el log al cliente"
    // 4º El texto del archivo lo guarda en el buffer
    // 5º Cierra el archivo
    string getLog()
    {
        ifstream archivo;
        archivo.open("c://Users//Gonzalo//UNLa//3º año//Redes y Comunicaciones//TP Redes//server.log.txt", ios::in);
        if(archivo.fail())
        {
            cout << "ERROR: No pudo abrirse el log" << endl;
            exit(1);
        }
        string texto;
        string textoCompleto;
        while(!archivo.eof())
        {
            getline(archivo, texto);
            textoCompleto = textoCompleto + texto + "#";
        }
        archivo.close();
        return textoCompleto;
    }

    void agregarTextoLog(char texto[])
    {
        ofstream archivo;
        archivo.open("c://Users//Gonzalo//UNLa//3º año//Redes y Comunicaciones//TP Redes//server.log.txt", ios::app);
        if(archivo.fail())
        {
            cout << "No pudo abrirse el archivo." << endl;
            exit(1);
        }
        char aux[130];
        memset(aux, 0, sizeof(aux));
        this->getCurrentDateAndHour(aux);
        strcat(aux, ": ");
        strcat(aux, texto);
        archivo << aux << endl;
        archivo.close();
    }

    // Retorna Nº (+ - * / ^ !)
    int cantidadDeOperadores(char buffer[])
    {
        int cantidadOp = 0;
        for(int i=0;i<strlen(buffer);i++)
        {
            if(buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '*' || buffer[i] == '/' || buffer[i] == '!' || buffer[i] == '^')
            {
                cantidadOp++;
            }
        }
        return cantidadOp;
    }

    // Valida que no haya recibido una cadena vacía
    bool validacion1(char buffer[])
    {
        return strlen(buffer) > 0;
    }

    // Valida que solo haya dígitos y operaciones
    // Si falló guarda en caracteresIncorrectos[] el 1º caracter incorrecto encontrado en el buffer
    bool validacion2(char buffer[], char caracteresIncorrectos[])
    {
        int i = 0;
        int posicionCaracterInvalido=0;
        bool caracteresValidos = true;
        while(i<strlen(buffer) && caracteresValidos)
        {
            // Si es distinto de 0..9, + - * / ^ !
            if(buffer[i] != '0' && buffer[i] != '1' && buffer[i] != '2' && buffer[i] != '3' && buffer[i] != '4' && buffer[i] != '5' && buffer[i] != '6' && buffer[i] != '7' && buffer[i] != '8' && buffer[i] != '9' && buffer[i] != '+' && buffer[i] != '-' && buffer[i] != '*' && buffer[i] != '/' && buffer[i] != '^' && buffer[i] != '!')
            {
                posicionCaracterInvalido=i;
                caracteresValidos = false;
            }
            i++;
        }
        // Si hay algún caracter inválido
        if(!caracteresValidos)
        {
            caracteresIncorrectos[0] = buffer[posicionCaracterInvalido];
        }
        return caracteresValidos;
    }

    // Valida que comience con un '+' o con 'n'.
    // Si valido = false, guarda en caracteresIncorrectos el operador y el número siguiente
    bool validacion3(char buffer[], char caracteresIncorrectos[])
    {
        bool valido = buffer[0] == '+' || (buffer[0] >= '0' && buffer[0] <= '9');
        if(!valido)
        {
            caracteresIncorrectos[0] = buffer[0];
            caracteresIncorrectos[1] = buffer[1];
        }
        return valido;
    }

    // Valida que si hay un '!' esté en la última posición
    // Si falla, guarda en caracteresIncorrectos[] el numero anterior (si existe), el operador y el numero siguiente
    bool validacion4(char buffer[], char caracteresIncorrectos[])
    {
        int i=0;
        int pos = -1;
        while(i<strlen(buffer) && pos==-1) { // Si lo encuentra, le pasa la posición. Si no lo encuentra pos=-1
            if(buffer[i] == '!') {
                pos = i;
            }
            i++;
        }
        // Si hay factorial y no está en la última posición
        if(pos != (-1) && pos != strlen(this->buffer)-1)
        {
            // Si está en la 1º posición del buffer, solo guarda el operador y el número siguiente
            if(pos == 0)
            {
                caracteresIncorrectos[0] = buffer[pos];
                caracteresIncorrectos[1] = buffer[pos + 1];
            }
            // Si no está en la 1º posición, guarda el número anterior, el operador y el número siguiente
            else
            {
                caracteresIncorrectos[0] = buffer[pos - 1];
                caracteresIncorrectos[1] = buffer[pos];
                caracteresIncorrectos[2] = buffer[pos + 1];
            }
        }
        // TRUE si no hay factorial || el factorial está en la última posición
        return pos == (-1) || pos == strlen(buffer)-1;
    }

    // Valida que no haya una operación en el último caracter y que, si la hay, que sea factorial
    bool validacion5(char buffer[])
    {
        return (buffer[strlen(buffer) - 1] >= '0' && buffer[strlen(buffer) - 1] <= '9') || buffer[strlen(buffer) - 1] == '!';
    }

    // Valida que no se esté dividiendo por 0
    bool validacion6(char buffer[])
    {
        int i=0;
        bool hayDivisor = false;
        bool divisorValido = false;
        while(i<strlen(buffer) && !hayDivisor)
        {
            if(buffer[i] == '/')
            {
                hayDivisor = true;
            }
            i++;
        }
        if(!hayDivisor)
            {
            divisorValido = true;
        }
        while(i<strlen(buffer) && !divisorValido)
        {
            if(buffer[i] != '0')
            {
                divisorValido = true;
            }
            i++;
        }
        return divisorValido;
    }

    // Valida que no hayan 2 operadores juntos
    // Si falla guarda en caracteresIncorrectos[] todo el buffer a partir del 1º operador
    bool validacion7(char buffer[], char caracteresIncorrectos[])
    {
        int i=0;
        int posicionPrimerOperador=0;
        bool valido = true;
        while(this->cantidadDeOperadores(buffer) == 2 && i<strlen(buffer)-1 && valido)
        {
            if(buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '*' || buffer[i] == '/' || buffer[i] == '!' || buffer[i] == '^')
            {
                if(buffer[i+1] == '+' || buffer[i+1] == '-' || buffer[i+1] == '*' || buffer[i+1] == '/' || buffer[i+1] == '!' || buffer[i+1] == '^')
                {
                    posicionPrimerOperador = i;
                    valido = false;
                }
            }
            i++;
        }
        if(!valido)
        {
            // j comienza en la posición del primer operador y recorre hasta el final. Va guardando en las
            // posiciones j-i [0, 1, 2...]
            for(int j=posicionPrimerOperador;j<strlen(buffer);j++)
            {
                caracteresIncorrectos[j-posicionPrimerOperador] = buffer[j];
            }
        }
        return valido;
    }

    // Valida que solo haya una operación
    // Si hay 3 operadores ya está mal
    // Si hay 2 pero buffer[0] = '+' está BIEN
    // Retorna verdadero si hay NINGÚN OPERADOR || UN OPERADOR || DOS OPERADORES DE LOS CUALES UN '+'
    // EN buffer[0]
    bool validacion8(char buffer[])
    {
        return this->cantidadDeOperadores(buffer) == 0 || this->cantidadDeOperadores(buffer) == 1 || (this->cantidadDeOperadores(buffer) == 2 && buffer[0] == '+');
    }

    // Si el número está mal ingresado retorna 0
    int factorial(int num)
    {
        int resultado = 0;
        if(num==0) {
            resultado = 1;
        }
        if(num>0) {
            resultado = 1;
            for(int i=num;i>=1;i--) {
                resultado *= i;
            }
        }
        return resultado;
    }

    int realizarCalculo(char buffer[]) {

        int resultado = 0;
        char numero1[20];
        char numero2[20];
        memset(numero1, 0, sizeof(numero1)); // Limpio las cadenas de caracteres (por las dudas).
        memset(numero2, 0, sizeof(numero2));
        char operacion = ' ';
        int i=0, j=0;
        if(buffer[0]=='+') {
            i++; // Si en la primera posición hay un '+', el operando comienza en la segunda.
        }

        // Si es un solo número o un solo número precedido por un '+'
        if(this->cantidadDeOperadores(buffer) == 0 || (this->cantidadDeOperadores(buffer) == 1 && buffer[0]=='+')) {
            resultado = atoi(buffer);
        // +123+234 || 123+234
        } else if (((this->cantidadDeOperadores(buffer) == 1 && (buffer[0]>='0'&&buffer[0]<='9')) || (this->cantidadDeOperadores(buffer) == 2 && buffer[0]=='+')) && buffer[strlen(buffer) - 1]!='!'){
            while(buffer[i]>='0' && buffer[i]<='9') {
                numero1[j] = buffer[i];
                i++;
                j++;
            }
            operacion = buffer[i]; // el iterador queda en el operador al salir del while. Le paso el operador.
            i++; // El iterador pasa al primer número del siguiente operando
            j=0; // Iterador para ir accediendo a las posiciones de numero2[] arrancando desde la 0
            while(i<strlen(buffer)) {
                numero2[j] = buffer[i];
                j++;
                i++;
            }
            switch(operacion){ // ''operacion' contiene el operador ingresado. Según cual sea, hace una operación u otra
                case '+':
                    resultado = atoi(numero1) + atoi(numero2);
                break;
                case '-':
                    resultado = atoi(numero1) - atoi(numero2);
                break;
                case '*':
                    resultado = atoi(numero1) * atoi(numero2);
                break;
                case '/':
                    resultado = atoi(numero1) / atoi(numero2);
                break;
                case '^':
                    resultado = pow(atoi(numero1), atoi(numero2));
                break;
            }
        } else { // Si es FACTORIAL
            while(buffer[i]!='!') { // Guarda el número (sin el operador) en número 1
                numero1[j] = buffer[i];
                j++;
                i++;
            }
            // Lo convierte a 'int' con función atoi, hace el factorial y lo guarda en resultado
            resultado = this->factorial(atoi(numero1));
        }
        return resultado;
    }

    // 1º Recibe el mensaje del cliente y lo muestra por consola
    // Chequea las validaciones. Si alguna falla, se lo informa al usuario.
    string Recibir()
    {
      recv(client, buffer, sizeof(buffer), 0);
      if(strlen(buffer)<=20)
      {
          // caracteresIncorrecto = contiene ¿qué estuvo mal en la operacion?. Ejemplo: '*/4'
          char caracteresIncorrectos[10];
          memset(caracteresIncorrectos, 0, sizeof(caracteresIncorrectos));
          cout << "El cliente envia la operacion: " << buffer << endl;

          // Guarda lo que manda el cliente en el log
          char textoLog[100];
          memset(textoLog, 0, sizeof(textoLog));
          strcpy(textoLog, "Cliente envia la operacion: ");
          strcat(textoLog, this->buffer);
          this->agregarTextoLog(textoLog); // Archivo = [dd/mm/aaaa hh:mm]: Cliente envía la operación <operacion>
          memset(textoLog, 0, sizeof(textoLog));

          // Guarda en texto log 'El servidor envía la respuesta...
          strcpy(textoLog, "Servidor envia la respuesta: ");

          // RECIBIÓ UNA CADENA VACÍA...
          if(!this->validacion1(this->buffer))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "La operacion debe tener entre 1 y 20 caracteres.");
              strcat(textoLog, "Error");
          }
          // ENCONTRÓ UN CARACTER QUE NO ES UN DÍGITO || OPERADOR
          else if(!this->validacion2(this->buffer, caracteresIncorrectos))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, se encontro un caracter no contemplado: ");
              strcat(this->buffer, caracteresIncorrectos);
              strcat(textoLog, "Error");
          }
          // EL PRIMER CARACTER NO ES '+' || 'n'
          else if(!this->validacion3(this->buffer, caracteresIncorrectos))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada. ");
              strcat(this->buffer, caracteresIncorrectos);
              strcat(textoLog, "Error");
          }
          // ERROR EN LA CANTIDAD DE OPERADORES
          else if(!this->validacion4(this->buffer, caracteresIncorrectos))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada. ");
              strcat(buffer, caracteresIncorrectos);
              strcat(textoLog, "Error");
          }
          // ENCONTRÓ UN OPERADOR DISTINTO A '!' EN EL ÚLTIMO CARACTER
          else if(!this->validacion5(this->buffer))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada.");
              strcat(textoLog, "Error");
          }
          // EL DIVISOR = 0
          else if(!this->validacion6(this->buffer))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada.");
              strcat(textoLog, "Error");
          }
          // ENCONTRÓ 2 OPERADORES JUNTOS
          else if(!this->validacion7(this->buffer, caracteresIncorrectos))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada. ");
              strcat(this->buffer, caracteresIncorrectos);
              strcat(textoLog, "Error");
          }
          // ENCONTRÓ UN '!' NO EN LA ÚLTIMA POSICIÓN
          else if(!this->validacion8(this->buffer))
          {
              memset(buffer, 0, sizeof(buffer));
              strcpy(buffer, "No se pudo realizar la operacion, la operacion esta mal formada.");
              strcat(textoLog, "Error");
          }
          else {
              char auxBuffer[21];
              strcpy(auxBuffer, buffer); // Guarda el mensaje de CLIENTE en un buffer auxiliar
              memset(buffer, 0, sizeof(buffer)); // Limpia el buffer principal
              sprintf(this->buffer, "%d", this->realizarCalculo(auxBuffer)); // Guarda en 'buffer' el resultado
              strcat(textoLog, this->buffer);
              memset(auxBuffer, 0, sizeof(auxBuffer)); // Limpia el buffer auxiliar
          }
          cout << "Respuesta enviada: " << buffer << endl << endl;
          this->agregarTextoLog(textoLog);
      }
      else if(strcmp(this->buffer,"111111111111111111111111111111")==0) // El cliente quiere ver el log
      {
          int i=0;
          string log = this->getLog(); // Pasa el texto del archivo al buffer
          if(log.length()>1023)
          {
             int j=0;
             i = log.length() - 1023;
             while(log[i]!='#')
             {
                 i++;
             }
             i++;
             while(i<log.length())
             {
                this->buffer[j] = log[i];
                j++;
                i++;
             }
          }
          else
          {
              for(int k=0;k<log.length();k++)
              {
                  this->buffer[k] = log[k];
              }
          }
          this->agregarTextoLog("Servidor envia log a Cliente");
      }
      else if(strcmp(this->buffer,"222222222222222222222222222222")==0) // Se detectó más de 2' de inactividad
      {
          this->agregarTextoLog("Servidor desconecta a Cliente por inactividad");
          cout << "Servidor desconecta a cliente por inactividad" << endl << endl;
          cout << "-----------------------------------------------------------------------" << endl << endl;
      }
      else if(strcmp(this->buffer,"333333333333333333333333333333")==0) // El cliente quiere desconectarse
      {
          this->agregarTextoLog("Cliente cierra sesion");
          cout << "Cliente cierra sesion" << endl << endl;
          cout << "-----------------------------------------------------------------------" << endl << endl;
      }
    }

    void Enviar()
    {
        send(client, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket()
    {
        closesocket(client);
    }

    void esperarNuevoCliente()
    {
        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);

        cout << "Esperando conexion..." << endl << endl;
        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            cout << "Un cliente se ha conectado!" << endl<< endl;
            this->agregarTextoLog("Cliente conectado");
        }
    }
};

int main()
{
  Server *Servidor = new Server();
  while(true)
  {
     Servidor->Recibir();
     // Si se cumplió la inactividad || el cliente se desconecta -> limpia su buffer,
     // desconecta al cliente y espera a otro
     if(strcmp(Servidor->buffer,"222222222222222222222222222222")==0 || strcmp(Servidor->buffer,"333333333333333333333333333333")==0)
     {
         memset(Servidor->buffer, 0, sizeof(Servidor->buffer));
         Servidor->CerrarSocket();
         Servidor->esperarNuevoCliente();
     }
     // Si el cliente sigue conectado, solo envía la respuesta
     else
     {
        Servidor->Enviar();
     }
  }
}
