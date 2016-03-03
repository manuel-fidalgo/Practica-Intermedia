#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
/*Mensajes de las difentes llamadas de */
void error(char zz){

   if(zz=='s') perror("Error en la llamada a signal\n");
   if(zz=='f') perror("Error en la llamda a fork\n");
   if(zz=='k') perror("Error en la llamada a kill\n");
    exit(-1);
}
/*Funcion te tipo handler, cambia el comportamiento de SIGUSR1 para que genere un numero aleatorio y termine*/
void calculaAleatoriosHandler(int sig){
    /*Definicion del manejador*/
    if (signal(SIGUSR1, calculaAleatoriosHandler)==SIG_ERR) 
        error('s');
    int aleat = calculaAleatorios(0,9);
    if(aleat<7){
        /*le viene bien la hora propuesta*/
        printf("Al pasajero (pid %d) le viene bien la hora ya que ha sacado un:%d\n",getpid(),aleat);
        exit(0);
    }
    else{
        /*Le viene mal la hora propuesta*/
        printf("Al pasajero (pid %d) le viene mal la hora ya que ha sacado un:%d\n",getpid(),aleat);
        exit(1);
    }
}

int calculaAleatorios(int min, int max) {
    return rand() % (max-min+1) + min;
}

/*Ambos handlers se limitan a ignorar la señal, solo para desbloquear el proceso que esta en pause*/
/*Funcion tipo handler, señal que van a recibir cada uno de los acompañantes*/
void confirmacion_Cliente(int sig){
    if(signal(SIGUSR1,confirmacion_Cliente)==SIG_ERR)
        error('s');
}
/*Funcion de tipo handler señal que va a recibir el padre*/
void confirmacion_Padre(int sig){
    if(signal(SIGUSR2,confirmacion_Padre)==SIG_ERR)
        error('s');
}
void imprimirCliente(int pid, int i){
    printf("El cliente %d (pid %d) ha recibido la solicitud de confirmacion del conductor(pid %d)\n",i+1,pid,getppid());
}
void imprimirConductor(int pid,int i){
    printf("El conductor(pid %d) ha recibido la confirmacion del cliente %d\n",pid,i+1);
}

int main (){
    
    /*Proceso padre*/
    int i; /*Declaracion de la variable que se va a usar en los bucles*/
    srand(getpid()*time(NULL));    /*declaramos un generador de numeros aleatorios*/
    int numpasajeros = calculaAleatorios(0,4);
    printf("%d personas han respondido al anuncio\n",numpasajeros);
    if(numpasajeros==0) return 0;
    pid_t p;
    int noNosVamos = 1;
    int rondas = 0;

    /*Bucle va a ejecutarse hasta que todos los pasajeros esten de acuerdo*/
    while(noNosVamos){
        /*Bucle que va a crear todos los procesos consultas*/
        /*Bucle que va a hacer que el padre envie una señal a cada proceso cunsulta */
        int contador=0;
        rondas++; 
        for(i=0; i<numpasajeros; i++){    
            p = fork();
            int status;
            /*En caso de que la llamada a fork produzca alguna excepcion*/
            if(p==-1) error('f');
            if(p==0){
                /*Codigo que van a ejecutar cada hijo*/
                srand(getpid()); 
                /*Cada vez que se reciba SIGUSR1 se ejecutara calculaAleatorioHandler*/
                if (signal(SIGUSR1, calculaAleatoriosHandler) == SIG_ERR) error('s');
                pause(); /*El proceso consulta se para hasta que reciba una señal*/
                /*Cuando se reciba la señal al hijo se llmara a la funcion handler y el proceso terminara con 0 o con 1*/
            }
            else{
                /*Codigo del padre*/

                sleep(1);   /*Para que le tiempo al proceso hijo a ejecutar el manejador y para faciitar la lectura de la ejecucion*/
                if(kill(p,SIGUSR1)!=0) error('k');
                wait(&status);
                int exitvalue = WEXITSTATUS(status);
                
                if(exitvalue==0){
                    printf("El conductor (pid %d) le ha propuesto una hora y al pasajero %d (pid %d) le viene bien (terminado con %d)\n",getpid(),(i+1),p,exitvalue);
                    contador++;
                }else{
                    printf("El conductor (pid %d)  le ha propuesto una hora y al pasajero %d (pid %d) le viene mal (terminado con %d)\n",getpid(),(i+1),p,exitvalue);
                    printf("Comenzamos otra ronda de consultas\n");
                    /*Para evitar que si hay uno que no esta de acuerdo se siga con la ronda*/
                    break;
                }
                
            }
            /*No es necesario ya que si alguno no estubiera de acuerdo saldria del bucle*/
            if(contador == numpasajeros) noNosVamos=0; /*Salimos el bloque while*/
        }
        /*Acaba el bucle del las consultas,ronda de consultas*/
        printf("Ronda %d completada\n",rondas);
    }
    printf("Todos nos hemos puesto de acuerdo en %d ronda/s\n",rondas);
    /*Parte del codigo que pide las confirmaciones*/
    /*Redefinimos la señal SIGUSR2 que van a enviar los hijos*/
    if(signal(SIGUSR2,confirmacion_Padre)!=0) error('s');
    for(i=0;i<numpasajeros;i++){
        /*Reutilizamos la variable p para el fork*/
        p = fork();
        if(p==-1){
            error('f');
        }
        if(p==0){
            /*Codigo de los hijos*/
            if(signal(SIGUSR1,confirmacion_Cliente)==SIG_ERR) error('s');

            pause();
            imprimirCliente(getpid(),i);

            if (kill(getppid(),SIGUSR2)!=0) error('k');

            exit(0);
        }else{
            /*Codigo del padre*/
            sleep(1); /*Para evitar que se ejecute el envio de señal antes de que se ejecute el manejador*/
            if(kill(p,SIGUSR1)!=0) error('k');
            /*Hace un pause() esperando por la señal de confimacion que le va a mandar e hijo*/
           pause();
           imprimirConductor(getpid(),i);
        }
    }
    printf("Nos vamos.\n");
return 0;
}
    