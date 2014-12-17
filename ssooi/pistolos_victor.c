/*********************************************************************
**	AUTORES:						DNI:							**
**		VÍCTOR BARRUECO GUTIÉRREZ	53518348-Q						**
**		ARTURO BALLEROS ALBILLO    	70912222-W						**
**			Práctica SSOOi, Grado en ingeniería informática (2014)	**
*********************************************************************/
/*Pistoleros de Kentucky

1.-Enunciado.
El programa que hay que presentar constará de un único fichero fuente de nombre pistolos.c. La correcta compilación de dicho programa, producirá un fichero ejecutable, cuyo nombre será obligatoriamente pistolos. Respetad las mayúsculas/minúsculas de los nombres, si las hubiere.

La ejecución del programa creará una serie de procesos que representarán una partida del problema del pistolero.

En dicho problema, un número determinado de pistoleros del oeste norteamericano se colocan en círculo. Cada uno de ellos elige al azar a un compañero. A la de tres, disparan todos al unísono. Algunos pistoleros se libran, algunos reciben uno o más disparos. Se retiran los cadáveres, los supervivientes cierran el corro y comienza una nueva ronda de elección y disparos. La historia puede acabar de dos únicos modos, con la supervivencia de uno o de ningún pistolero. Matemáticamente, lo interesante es calcular la probabilidad de que sobreviva un pistolero dependiendo del número inicial de ellos. Esto no nos interesará en esta práctica.

En la práctica, cada pistolero vendrá representado por un proceso, hijos todos del proceso original, que ejecuta main. A este proceso original se le pasará por la línea de órdenes un número entero, correspondiente con el número inicial de pistoleros que forman la partida. El número estará comprendido entre 3 y 128.

A continuación, tendrá tantos hijos como indique dicho número. El padre regula el funcionamiento de las rondas hasta llegar al final de ellas. En el caso de que, al final, quede un proceso pistolero vivo, lo matará, tomará su código de retorno y pondrá el resultado final de la partida. El padre sólo matará, de tener que hacerlo, a este último pistolero. No participa en las rondas disparando.

2.-Funcionamiento de una ronda de disparos
En cada ronda de disparos, el proceso padre imprime en un fichero los PIDs de los pistoleros que están vivos para que ellos lo puedan leer y sepan a quiéni pueden disparar. Escribe en la pantalla lo equivalente a este ejemplo:

*** COMIENZA LA RONDA NUMERO 7 ***
*** PIDs: 1234 1689 1748 2222

Acto seguido, enviará una señal SIGUSR1 a cada uno de ellos para indicarles que pueden elegir al compañero y dispararle.

Los pistoleros eligen, entonces, a otro pistolero al azar. No pueden dispararse a sí mismos. Anuncian el PID del pistolero elegido por la pantalla y envian una señal SIGTERM a dicho pistolero, para matarlo.


La salida por la pantalla siguiendo el ejemplo anterior podría ser:

1234->1689
2222->1689
1748->2222
1689->1748

Debe aparecer, por lo tanto, una línea por cada proceso indicando a qué proceso ha disparado. No tienen por qué aparecer en orden. Dependerá de cómo reparta el Sistema Operativo la CPU. Y también deben disparar todos los pistoleros, incluso aunque les haya llegado un disparo antes.

El proceso padre hará tantas llamadas al sistema waits como procesos pistoleros había antes de disparar y, de este modo, sabrá qué procesos han muerto a causa de los disparos.

Como puede que algún proceso quede vivo y, para que el proceso padre no espere eternamente, se necesita algo que le despierte de la espera bloqueante en que le sumen los waits. Esto lo logra el propio proceso padre, mediante una llamada previa a a alarm antes de dormirse. La llamada hará que el Sistema Operativo le envíe una señal SIGALRM cuando pase un segundo.

El proceso padre sabe, al final, cuántos procesos han quedado vivos y puede iniciar una nueva ronda en el caso de que sea necesario.


3.-Restricciones

    Se deberán usar llamadas al sistema siempre que sea posible, a no ser que se especifique lo contrario.
    No está permitido usar la función de biblioteca system, salvo indicación explícita en el enunciado de la práctica.
    No se puede suponer que los PIDs de los procesos de una ristra van a aparecer consecutivos. Puestos en plan exquisito, ni siquiera podemos suponer que estarán ordenados de menor a mayor (puede ocurrir que se agoten los PIDs y retome la cuenta partiendo de cero).
    No está permitido el uso de ficheros, tuberías u otro mecanismo externo para transmitir información entre los procesos, salvo que se indique en el enunciado.
    Supondremos un límite máximo de pistoleros iniciales igual a 128 procesos. Este límite os puede servir para no tener que usar memoria dinámica si no lo deseáis.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>  //libreria de señales
#include <fcntl.h>	 //libreria de ficheros
#include <string.h>  //libreria de cadenas

//#define DEBUG

#define CHILD_MAX 12000 //Número máximo de hijos (Recomendados en la práctica)

//Signal Handler
void sh(int sn){
    if (sn == SIGUSR1){
    }
    if (sn == SIGUSR2){
    }
    if (sn == SIGALRM){
    }
    if (sn == SIGTERM){
    	sleep(0);
    	exit (0);
    }

}//End of sh



void killAll(int numChild,int *pid){
	int i;
	for(i=0; i<numChild; i++){
		kill(pid[i], SIGTERM);
	}
}



//Imprime ayuda por pantalla
int printHelp(){
    fprintf(stderr,"\nUso del programa:\n");
    fprintf(stderr,"pistolero [numero de pistoleros]\n");
    fprintf(stderr,"\tNúmero mínimo de pistoleros: 3\n");
    fprintf(stderr,"\tNúmero máximo de pistoleros: %d\n",CHILD_MAX);
    fprintf(stderr,"\tPor defecto número de pistoleros = 3\n\n");
    return 1;
}//End of printHelp



int main (int argc, char* argv[]){
	pid_t pid[CHILD_MAX]={0}, pidDead[CHILD_MAX]={0}, pidTemp[CHILD_MAX]={0};	//Arrys para los PID's
	int numChild=3, numDead=0;		    //Numero de hijos
    int victim=-1;						//Seleccionamos pid de la próxima victima
	int fd;                		    	//Descriptor de fichero
	int wr;                      		//Comprobacion de write
	char temp[8]; 		                //Convertir integer to char
	char *buffer;           		    //Puntero al fichero en memoria
	int size;              		  		//Tamaño del fichero pids.txt
	int rd;		               		    //Bytes leidos del archivo
	int i, j, k;                   		//Variable auxiliar
	static struct sigaction sen_USR1;
	static sigset_t newChildMask;		//Aquí van las mascaras de las señales
//	static sigset_t oldChildMask;	    //Aquí van las mascaras de las señales
	static sigset_t sigusr1Mask;        //Mascara para SIGUSR1
	static sigset_t sig_usr1Mask;      	//Mascara para SIGTERM
	static sigset_t alrmMask;           //Mascara para SIGALRM
	static sigset_t sigchldMask;        //Mascara para SIGCHLD
	static sigset_t empty;              //Mascara vacia
	static sigset_t full;				//Mascara llena
	int isDead;							//Informa al padre de que el proceso está muerto
	int round=0;                        //Contiene el numero de ronda
	int rtrn;							//Valor devuelto por los pids
	char print[32];



	//Comprobamos la llamada al programa
    if(argc>2){
        return printHelp();
    }
    //Cargamos el número de pistoleros
    if(argc == 2){
    	numChild=atoi(argv[1]);
    }
    //Comprobamos le número de pistoleros
    if(numChild<3 || numChild>CHILD_MAX){
		return printHelp();
    }



	// Gestionaremos las señales que se envíen a todos los procesos

	if (sigemptyset(&empty) == -1){
		fprintf(stderr,"\n\e[31mERROR al limpiar la mascara vacia.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigfillset(&full) == -1){
		fprintf(stderr,"\n\e[31mERROR al llenar la mascara llena.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	sen_USR1.sa_handler=sh;	//Cambio la señal de la manejadora
	sen_USR1.sa_flags=0;
	sen_USR1.sa_mask=full;


	// Avisamos al sistema operativo de que gestionaremos las señales
	if (sigaction(SIGUSR1,&sen_USR1,NULL) == -1){
			perror("\e[31mPADRE: sigaction\e[0m");
			killAll(numChild, pid);
			return 1;
	}
	if (sigaction(SIGUSR2,&sen_USR1,NULL) == -1){
			perror("\e[31mPADRE: sigaction\e[0m");
			killAll(numChild, pid);
			return 1;
	}
	if (sigaction(SIGALRM,&sen_USR1,NULL) == -1){
			perror("\e[31mPADRE: sigaction\e[0m");
			killAll(numChild, pid);
			return 1;
	}
	if (sigaction(SIGTERM,&sen_USR1,NULL) == -1){
			perror("\e[31mPADRE: sigaction\e[0m");
			killAll(numChild, pid);
			return 1;
	}


	//Modifico y preparo la mascara que debe tener el hijo

	if (sigfillset(&newChildMask) == -1 ){			//Vacio la mascara para el hijo
		fprintf(stderr,"\n\e[31mERROR al llenar la mascara del hijo.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigdelset(&newChildMask,SIGUSR2) == -1 ){	//Añado la señal de SIGUSR2
		fprintf(stderr,"\n\e[31mERROR al añadir mascara de SIGUSR2 para el hijo.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigdelset(&newChildMask,SIGTERM) == -1 ){	//Añado la señal de SIGTERM
		fprintf(stderr,"\n\e[31mERROR al añadir mascara de SIGTERM para el hijo.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigemptyset(&sigusr1Mask) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al limpiar la mascara de SIGUSR1.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigaddset(&sigusr1Mask,SIGUSR1) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al añadir mascara de SIGUSR1.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigfillset(&sig_usr1Mask) == -1){
		fprintf(stderr,"\n\e[31mERROR al llenar la mascara de sin SIGUSR1.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigdelset(&sig_usr1Mask,SIGUSR1) == -1){
		fprintf(stderr,"\n\e[31mERROR al quitar mascara de SIGUSR1.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigprocmask(SIG_BLOCK, &full, NULL) == -1){
		fprintf(stderr,"\n\e[31mERROR al bloquear todas las sennales.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigfillset(&alrmMask) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al limpiar la mascara de SIGALRM.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigdelset(&alrmMask,SIGALRM) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al añadir mascara de SIGALRM.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigemptyset(&sigchldMask) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al limpiar la mascara de SIGCHLD.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}
	if (sigaddset(&alrmMask,SIGCHLD) == -1 ){
		fprintf(stderr,"\n\e[31mERROR al añadir mascara de SIGCHLD.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}




	//Creacion de los pistoleros
	for(i=0; i<numChild; i++){
		pid[i]=fork();
		switch(pid[i]){
			case -1:
				perror("\e[31mError de fork().\e[0m");
				killAll(numChild, pid);
    			exit(1);
			case 0:
				srand(getpid());

				//Los hijos entran en un bucle infinito
				for(;;){
					//Pauso el proceso hasta que reciba SIGUSR1
					sigsuspend(&sig_usr1Mask);	//Pauso el proceso hasta que reciba SIGUSR1

					fd=open("alive", O_RDONLY);

					//Compruebo el tamaño del fichero
					size=lseek(fd,0,SEEK_END);
					if (size<0){
						fprintf(stdout,"\e[31mERROR, en la lectura del fichero 'alive'.\e[0m\n");
						killAll(numChild, pid);
						return 1;
					}

					//Situo el cursor al principio del archivo
					k=lseek(fd,0,SEEK_SET);
					if (k!=0){
						fprintf(stdout,"\e[31mERROR, no estoy al principio del fichero.\e[0m\n");
						killAll(numChild, pid);
						return 1;
					}

					//Reservo memoria para leer el fichero
					buffer=malloc(sizeof(char)*size);
					rd=read(fd, buffer, size);
					if (rd != size){
						fprintf(stdout,"\e[31mERROR, en la lectura del fichero por los hijos.---->%d\e[0m\n",rd);
						killAll(numChild, pid);
						return 1;
					}

					//Cierro el fichero
					if (-1 == close(fd)){
						fprintf(stdout,"\e[31mERROR, en el cierre del fichero por el hijo.\e[0m\n");
						killAll(numChild, pid);
						return 1;
					}

					//Leemos el archivo
					for (k=0; k<8; k++){
						temp[k]='\0';
					}

					int brother=0;	//Vamos a contar los hermanos a los que podemos disparar

					j=0;
					for (k=0; k<size; k++){
						if (buffer[k] != '*'){
							temp[j] = buffer[k];
							j++;
						}
						else{
							pid[brother] = atoi(temp);
							if (pid[brother] != getpid()){
								brother++;
							}
							for (j=0; j<8; j++){
								temp[j]='\0';
							}
							j=0;
						}
					}

					victim = (rand() % brother);

					sprintf(print,"%d->%d\n", getpid(), pid[victim]);
    				write(1, print, strlen(print));
					kill (pid[victim], SIGTERM);	// Disparo
					sigsuspend(&newChildMask);
				}
		}
	}
	if (sigprocmask(SIG_SETMASK, &alrmMask, NULL) == -1){
		fprintf(stdout,"\n\e[31mERROR al desbloquear SIGALRM.\e[0m\n");
		killAll(numChild, pid);
		return 1;
	}



	//Bucle de rondas
	while(numChild > 1){
		round++;
		sprintf(print,"\nRonda %d:\tHay %d pistoleros vivos\n", round, numChild);
    	write(1, print, strlen(print));

		//Abro o creo si no existe un fichero para pids
		fd = open ("alive",O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (fd == -1){
			fprintf(stdout,"\e[31mERROR,no se puede abrir el fichero 'alive'.\e[0m\n");
			killAll(numChild, pid);
			return 1;
		}

		//Escribo los pids de mis hijos
		for (i=0; i<numChild; i++){
				sprintf(temp, "%d*", pid[i]);
				wr = write(fd, temp, strlen(temp)*sizeof(char));
			if (wr == -1){
				fprintf(stdout,"\e[31mERROR, el escribir el pid en el fichero.\e[0m\n");
				killAll(numChild, pid);
				return 1;
			}
		}
		//Cierro el fichero
		if (-1 == close(fd)){
			fprintf(stdout,"\e[31mERROR, al cerrar el fichero 'alive'.\e[0m\n");
			killAll(numChild, pid);
			return 1;
		}

		//Despertamos a los hijos
		for (i=0; i<numChild; i++){
			kill (pid[i],SIGUSR1);
		}

		//Esperamos a ver quien a muerto
		alarm(1);
		numDead = 0;
		for (i = 0; i < numChild; i++){
			rtrn = wait(NULL);
			if (rtrn != -1){
				pidDead[numDead]= rtrn;
				numDead++;
			}
			else{
				perror("Wait...");
				break;
			}
		}
		sprintf(print,"\nNumero inicial: %d  Número muertos:%d\n", numChild, numDead);
    	write(1, print, strlen(print));

		//Variable temporal para ver los hijos vivos
		int temp2 = 0;
		for (k = 0; k < numChild; k++){
			isDead = 0;
			//Comprobamos que hijos estan muertos
			for (j = 0; j < numDead; j++){
				if (pid[k] == pidDead[j]){
				isDead = 1;
				}
			}
			//Si no estan muertos los almacenamos en una temporal que se irá reescribiendo
			if (isDead == 0) {
				pidTemp[temp2]=pid[k];
				temp2++;
			}
		}


		if (temp2 == 1){
			kill(pidTemp[0], SIGTERM);
			wait(&rtrn);
			sprintf(print,"El vencedor ha sido %d\n", pidTemp[0]);
    		write(1, print, strlen(print));
			exit (0);
		}
		else if (temp2 == 0){
			sprintf(print,"\nHan muerto todos\n");
    		write(1, print, strlen(print));
			exit (0);
		}
		else{//Necesitamos más rondas
			//Sustituyamos los antiguos en el array y les comunicamos que siguen vivos
			for (k = 0; k < temp2; k++){
				pid[k]=pidTemp[k];
				kill(pidTemp[k], SIGUSR2);
			}
			numChild = temp2;

			alarm(1);
			sigsuspend(&alrmMask);
			//exit (0);/*DEBUG*/
		}
	}//Fin de rondas
	return 0;
}//End of main
