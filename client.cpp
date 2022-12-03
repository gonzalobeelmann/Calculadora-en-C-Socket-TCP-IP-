#include <iostream>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

using namespace std;

class Client{
public:

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];

    // Cada vez que el usuario ingrese algo, se validará que ese momento
    // no haya pasado 2' del último ingreso. Si pasaron 2', desconecta
    // al cliente. Si no, actualiza los atributos
    int anioUltimoAcceso;
    int mesUltimoAcceso;
    int diaUltimoAcceso;
    int horaUltimoAcceso;
    int minutoUltimoAcceso;
    int segundoUltimoAcceso;

    Client()
    {
        char puerto[6];
        memset(puerto, 0, sizeof(puerto));
        cout << "Ingrese en numero de puerto al que desea conectarse" << endl;
        cout << "Respuesta: ";
        gets(puerto);
        while(!this->puertoValido(puerto))
        {
            cout << endl << "-----------------------------------------------------------------" << endl << endl;
            cout << "Error. Ingrese un puerto valido dentro del intervalo [1024;49151]" << endl;
            cout << "Respuesta: ";
            gets(puerto);
        }
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.10");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(puerto));
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
        system("cls");
        cout << "Conexion exitosa!" << endl;
        this->actualizarUltimoAcceso();
    }

    // Retorna verdadero si el puerto contiene 4 o 5 caracteres y va de 1024 a 49151
    bool puertoValido(char port[])
    {
        return (strlen(port) == 4 || strlen(port) == 5) && atoi(port) >= 1024 && atoi(port) <= 49151;
    }

    int Enviar()
    {
        char continuar[20];
        memset(continuar, 0, sizeof(continuar));
        bool cumplioTiempo = false;
        cout << "Escriba una operacion" << endl;
        cout << "Respuesta: ";
        gets(this->buffer);
        if(!this->chequearInactividad())
        {
            cumplioTiempo = true;
        }
        // Validación que no supere los 20 caracteres.
        if(!cumplioTiempo)
        {
            while(strlen(buffer)>20)
            {
                cout << endl << "-----------------------------------------------" << endl << endl;
                cout << "La operacion debe tener entre 1 y 20 caracteres" << endl << endl;
                memset(buffer, 0, sizeof(buffer));
                cout << "Escriba una operacion valida" << endl;
                cout << "Respuesta: ";
                gets(this->buffer);
                if(!this->chequearInactividad())
                {
                    this->solicitarDesconexionPorInactividad();
                    cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
                    cout << "Ingrese cualquier caracter para continuar: ";
                    gets(continuar);
                    cumplioTiempo = true;
                    system("cls");
                }
            }
            system("cls");
            strlwr(this->buffer);
            if(strcmp(this->buffer, "volver")!=0 && !cumplioTiempo) // Si no ingresó volver retorna -1
            {
                cout << "Su operacion: " << this->buffer << endl;
                send(server, buffer, sizeof(buffer), 0);
                return -1;
            }
            else if(strcmp(this->buffer, "volver")==0 && !cumplioTiempo)
            {
                return 1; // Si ingresó volver retorna 1
            }
        }
        else
        {
            return 0;
        }
        memset(buffer, 0, sizeof(buffer));
    }

    void Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);
        cout << "Resultado: " << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
    }

    void pedirLog()
    {
        // Carga el buffer con 30 caracteres, si el servidor recibe
        // 30, detecta que se está pidiendo el buffer
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "111111111111111111111111111111");
        system("cls");
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void imprimirLog()
    {
        recv(server, buffer, sizeof(buffer), 0);
        for(int i=0;i<sizeof(buffer);i+=1)
        {
            if(buffer[i]!='#')
            {
                cout << buffer[i];
            }
            else
            {
                cout << endl;
            }
        }
        memset(buffer, 0, sizeof(buffer));
    }

    // Retorna el minuto actual en el sistema
    int getCurrentMinute()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_min;
    }

    // Retorna el segundo actual en el sistema
    int getCurrentSecond()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_sec;
    }

    // Retorna el dia actual del sistema [1;31]
    int getCurrentDayOfMonth()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_mday;
    }

    // Retorna el mes actual del sistema [1;12]
    int getCurrentMonth()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_mon + 1;
    }

    // Retorna el año actual del sistema
    int getCurrentYear()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_year + 1900;
    }

    // Retorna la hora actual
    int getCurrentHour()
    {
        time_t localDateNow; // Declaro variable del tipo time_t
        time(&localDateNow); // Le guardo el tiempo actual del sistema
        struct tm *tiempo = localtime(&localDateNow); // Creo un puntero del tipo 'struct tm' que contendrá los datos del tiempo actual
        return tiempo->tm_hour;
    }

    // Valido si accedió en 2022 y en 2022 || en 2022 y 2023 (pueden no haber pasado 2').
    bool validarAnio()
    {
        return this->anioUltimoAcceso == this->getCurrentYear() || this->getCurrentYear() - this->anioUltimoAcceso == 1;
    }

    // Valida si accedió dentro del mismo mes || ultimo acceso en un mes y volvió a acceder en el siguiente
    bool validarMes()
    {
        bool valido = false;
        // Si había accedido en Diciembre y volvió a operar en Diciembre || Enero
        if(this->mesUltimoAcceso == 12 && (this->getCurrentMonth() == 12 || this->getCurrentMonth() == 1))
        {
            valido = true;
        }
        // Si el último acceso NO había sido en diciembre
        if(this->mesUltimoAcceso != 12)
        {
            // Si accedió en el mismo mes || accedió un mes después
            if(this->getCurrentMonth() - this->mesUltimoAcceso == 0 || this->getCurrentMonth() - this->mesUltimoAcceso == 1)
            {
                valido = true;
            }
        }
        return valido;
    }

    // Valida si el acceso actual y el anterior fueron el mismo día
    // o si el ultimo acceso fue un dia y volvio a acceder en el siguiente
    bool validarDia()
    {
        bool valido = false;
        // Si había accedido en ENERO, MARZO, MAYO, JULIO, AGOSTO, OCTUBRE || DICIEMBRE (tienen 31 días).
        if(this->mesUltimoAcceso == 1 || this->mesUltimoAcceso == 3 || this->mesUltimoAcceso == 5 || this->mesUltimoAcceso == 7 || this->mesUltimoAcceso == 8 || this->mesUltimoAcceso == 10 || this->mesUltimoAcceso == 12)
        {
            // Si había accedido el día 31 && seguimos en 31 o pasó a 1
            if(this->diaUltimoAcceso == 31 && (this->getCurrentDayOfMonth() == 31 || this->getCurrentDayOfMonth() == 1))
            {
                valido = true;
            }
            if(this->diaUltimoAcceso != 31)
            {
                if(this->getCurrentDayOfMonth() == this->diaUltimoAcceso || this->getCurrentDayOfMonth() - this->diaUltimoAcceso == 1)
                {
                    valido = true;
                }
            }
        }
        // Si había accedido en febrero y es un año bisiesto
        if(this->mesUltimoAcceso == 2 && this->anioUltimoAcceso % 4 == 0)
        {
            // Si había accedido el día 29 && seguimos en 29 o pasó a 1
            if(this->diaUltimoAcceso == 29 && (this->getCurrentDayOfMonth() == 29 || this->getCurrentDayOfMonth() == 1))
            {
                valido = true;
            }
            if(this->diaUltimoAcceso != 29)
            {
                if(this->getCurrentDayOfMonth() == this->diaUltimoAcceso || this->getCurrentDayOfMonth() - this->diaUltimoAcceso == 1)
                {
                    valido = true;
                }
            }
        }
        // Si había accedido en febrero y NO es un año bisiesto
        if(this->mesUltimoAcceso == 2 && this->anioUltimoAcceso % 4 != 0)
        {
            // Si había accedido el día 28 && seguimos en 28 o pasó a 1
            if(this->diaUltimoAcceso == 28 && (this->getCurrentDayOfMonth() == 28 || this->getCurrentDayOfMonth() == 1))
            {
                valido = true;
            }
            if(this->diaUltimoAcceso != 28)
            {
                if(this->getCurrentDayOfMonth() == this->diaUltimoAcceso || this->getCurrentDayOfMonth() - this->diaUltimoAcceso == 1)
                {
                    valido = true;
                }
            }
        }
        // Si había accedido en ABRIL || JUNIO || SEPTIEMBRE || NOVIEMBRE (tienen 30 días)
        if(this->mesUltimoAcceso == 4 || this->mesUltimoAcceso == 6 || this->mesUltimoAcceso == 9 || this->mesUltimoAcceso == 11)
        {
            // Si había accedido el día 30 && seguimos en 30 o pasó a 1
            if(this->diaUltimoAcceso == 30 && (this->getCurrentDayOfMonth() == 30 || this->getCurrentDayOfMonth() == 1))
            {
                valido = true;
            }
            if(this->diaUltimoAcceso != 30)
            {
                if(this->getCurrentDayOfMonth() == this->diaUltimoAcceso || this->getCurrentDayOfMonth() - this->diaUltimoAcceso == 1)
                {
                    valido = true;
                }
            }
        }
        return valido;
    }

    // Si accedió a la misma hora del ultimo acceso, retorna TRUE.
    // Si accedió 1h después del ultimo acceso, retorna TRUE
    bool validarHora()
    {
        bool valido = false;
        if(this->horaUltimoAcceso == 23)
        {
            if(this->getCurrentHour() == 23 || this->getCurrentHour() == 0)
            {
                valido = true;
            }
        }
        else
        {
            if(this->getCurrentHour() == this->horaUltimoAcceso || this->getCurrentHour() - this->horaUltimoAcceso == 1)
            {
                valido = true;
            }
        }
        return valido;
    }

    // Si no pasaron 2 el ultimo acceso, retorna true
    bool validar2Minutos()
    {
        bool valido = false;
        if(this->minutoUltimoAcceso == 58)
        {
            if(this->minutoUltimoAcceso == this->getCurrentMinute())
            {
                valido = true;
            }
            else if(this->getCurrentMinute() == 59)
            {
                valido = true;
            }
            else if(this->getCurrentMinute() == 0)
            {
                if(this->getCurrentSecond() <= this->segundoUltimoAcceso)
                {
                    valido = true;
                }
            }
        }
        else if(this->minutoUltimoAcceso == 59)
        {
            if(this->minutoUltimoAcceso == this->getCurrentMinute())
            {
                valido = true;
            }
            else if(this->getCurrentMinute() == 0)
            {
                valido = true;
            }
            else if(this->getCurrentMinute() == 1)
            {
                if(this->getCurrentSecond() <= this->segundoUltimoAcceso)
                {
                    valido = true;
                }
            }
        }
        else
        {
            if(this->getCurrentMinute() - this->minutoUltimoAcceso == 1 || this->getCurrentMinute() - this->minutoUltimoAcceso == 0)
            {
                valido = true;
            }
            else if(this->getCurrentMinute() - this->minutoUltimoAcceso == 2)
            {
                if(this->getCurrentSecond() <= this->segundoUltimoAcceso)
                {
                    valido = true;
                }
            }
        }
    }

    // Si todas las validaciones del time out están OK, retorna TRUE.
    bool chequearInactividad()
    {
        return this->validarAnio() && this->validarMes() && this->validarDia() && this->validarHora() && this->validar2Minutos();
    }

    // Le envía al server (30) 2...
    void solicitarDesconexionPorInactividad()
    {
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "222222222222222222222222222222");
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void solicitarDesconexion()
    {
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "333333333333333333333333333333");
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void actualizarUltimoAcceso()
    {
        this->anioUltimoAcceso = this->getCurrentYear();
        this->mesUltimoAcceso = this->getCurrentMonth();
        this->diaUltimoAcceso = this->getCurrentDayOfMonth();
        this->horaUltimoAcceso = this->getCurrentHour();
        this->minutoUltimoAcceso = this->getCurrentMinute();
        this->segundoUltimoAcceso = this->getCurrentSecond();
    }

    void CerrarSocket()
    {
       closesocket(server);
       WSACleanup();
    }
};

int main()
{
    desconexionPorInactividad: // Etiqueta. Se usa solo en caso de que se desconecte por inactividad
    Client *Cliente = new Client();
    // Una vez que el cliente se conecta al servidor, actualiza por 1º vez
    // su último acceso
    Cliente->actualizarUltimoAcceso();
    int volver = 0;
    char continuar[20];
    cout << endl << "Ingrese cualquier caracter para continuar: ";
    gets(continuar);
    system("cls");
    // Si superó los 2 minutos, se ejecuta solicitarDesconexionPorInactividad()
    // el cual carga (30) dos en el buffer y lo envía al servidor
    // se lo informa y, cuando ingresa un caracter,
    // Vuelve al inicio del programa.
    // Si no superó los 2 minutos, actualiza el último acceso y el flujo
    // del programa continúa con normalidad
    if(!Cliente->chequearInactividad())
    {
        Cliente->solicitarDesconexionPorInactividad();
        cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente." << endl << endl;
        cout << "Ingrese cualquier caracter para continuar: ";
        gets(continuar);
        system("cls");
        goto desconexionPorInactividad;
    }
    Cliente->actualizarUltimoAcceso();
    char opcion[60];
    while(opcion[0] != '3')
    {
        cout << "1) Realizar operacion" << endl;
        cout << "2) Registro de actividades" << endl;
        cout << "3) Cerrar sesion" << endl << endl;
        cout << "Respuesta: ";
        gets(opcion);
        if(!Cliente->chequearInactividad())
        {
            system("cls");
            Cliente->solicitarDesconexionPorInactividad();
            cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
            cout << "Ingrese cualquier caracter para continuar: ";
            gets(continuar);
            system("cls");
            goto desconexionPorInactividad;
        }
        Cliente->actualizarUltimoAcceso();
        // Si no ingresó un número dentro del intervalo [1;3]
        // se lo solicita de nuevo
        while(opcion[0] > '3' || opcion[0] < '1' || strlen(opcion)!=1)
        {
            cout << "---------------------------------" << endl << endl;
            cout << "Error. Ingrese una opcion valida" << endl << endl;
            cout << "Respuesta: ";
            gets(opcion);
            if(!Cliente->chequearInactividad())
            {
                system("cls");
                Cliente->solicitarDesconexionPorInactividad();
                cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
                cout << "Ingrese cualquier caracter para continuar: ";
                gets(continuar);
                system("cls");
                goto desconexionPorInactividad;
            }
        Cliente->actualizarUltimoAcceso();
        }
        system("cls");
        switch(opcion[0])
        {
            case '1':
                volver = Cliente->Enviar();
                // Si el método Enviar() retornó -1: Ingresó una OPERACIÓN
                // por lo tanto, recibe la respuesta, chequea la inactividad
                // y actualiza el último acceso
                if(volver == -1)
                {
                    Cliente->Recibir();
                    cout << endl << "Ingrese cualquier caracter para continuar: ";
                    gets(continuar);
                    system("cls");
                    if(!Cliente->chequearInactividad())
                    {
                        Cliente->solicitarDesconexionPorInactividad();
                        cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
                        cout << "Ingrese cualquier caracter para continuar: ";
                        gets(continuar);
                        system("cls");
                        goto desconexionPorInactividad;
                    }
                }
                else if(volver == 0) // Si supero el time out
                {
                    Cliente->solicitarDesconexionPorInactividad();
                    system("cls");
                    cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
                    cout << "Ingrese cualquier caracter para continuar: ";
                    gets(continuar);
                    system("cls");
                    goto desconexionPorInactividad;
                }
                volver = 0;
                Cliente->actualizarUltimoAcceso();
                break;
            case '2':
                Cliente->pedirLog(); // Carga (30) unos en el buffer y lo envía
                Cliente->imprimirLog();
                cout << "Ingrese cualquier caracter para continuar: ";
                gets(continuar);
                system("cls");
                if(!Cliente->chequearInactividad())
                {
                    Cliente->solicitarDesconexionPorInactividad();
                    cout << "Ha superado el limite de inactividad (2'). Ingrese nuevamente" << endl << endl;
                    cout << "Ingrese cualquier caracter para continuar: ";
                    gets(continuar);
                    system("cls");
                    goto desconexionPorInactividad;
                }
                Cliente->actualizarUltimoAcceso();
                break;
            case '3':
                Cliente->solicitarDesconexion(); // Carga (30) tres en el buffer y lo envía
                system("cls");
                cout << "Gracias por usar la aplicacion!" << endl;
                Cliente->CerrarSocket();
                break;
        }
    }
}
