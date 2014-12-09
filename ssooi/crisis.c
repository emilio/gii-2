/*
PRÁCTICA EVALUABLE #2, SSOO I, 2º GRADO INGENIERÍA INFORMATICA 2012-13
Autores: Juan Manuel Bécares Silva y Carlos Calvo Rodríguez.
*/

#include <stdio.h>        
#include <signal.h>        
#include <unistd.h>        
#include <time.h>        
#include <errno.h>        
#include <stdlib.h>        
#include <fcntl.h>        
#include <string.h>        
#include <sys/types.h>    
#include <sys/wait.h>    

//Estados de proceso
#define STATE_TERMINATE 0    //Terminacion. Envía SIGTERM a sus hijos y espera a 
							//que mueran.
#define STATE_WAIT 1
#define STATE_END 2            //Fin. Todos los hijos han muerto, imprimir mensaje 
							//final y morir.
#define STATE_RUNNING 3        //En funcionamiento normal.
#define STATE_WAIT_CHILD 4    //Esperando a que se complete la creacion de un hijo
#define STATE_INTERRUPT 5    //Interrupcion. Cuando se recibe SIGINT. Similar a 
							//STATE_TERMINATE, pero no modifica el archivo de 
							//control, ya que es el final del proceso.

//Nodo de lista enlazada.
struct ll_node {
	int pid;
	struct ll_node *next;    //Siguiente elemento.
};
typedef struct ll_node ll_node;

//Estructura de lista enlazada.
typedef struct ll_childs {
	ll_node *first;    //Primer elemento.
	ll_node *last;    //Ultimo elemento.
} ll_childs;

//Funciones de control de listas.
ll_childs* createList();            //Crear lista vacia.
int addChild(ll_childs*, int);        //Anyadir hijo a lista.
void removeChild(ll_childs*, int);    //Eliminar hijo de lista.
void printList(ll_childs*);            //Imprime lista. Solo para debug.



//Globales necesarias
/*
Son necesarias ya que deben ser accedidas desde la signal handler sh, que no
admite argumentos por parte del usuario.
*/
ll_childs *childs;            //Lista enlazada de hijos.
int state = STATE_RUNNING;     //Estado del proceso.


//Signal Handler 
void sh(int sn){
	if (sn == SIGCHLD){    
		int rw, ws;
		rw = wait(&ws);    
		removeChild(childs, rw);
	}
	if (sn == SIGINT){    
		state = STATE_INTERRUPT;
	}
	if (sn == SIGTERM){    
		if (state != STATE_INTERRUPT) state = STATE_TERMINATE;
	}
	if (sn == SIGUSR1 || sn == SIGALRM){ 
		if (state == STATE_WAIT_CHILD) state = STATE_RUNNING;
	}
}


void printHelp(){
	fprintf(stderr, "\nUso del programa:\n");
	fprintf(stderr, "crisis <max procesos> [velocidad]\n");
	fprintf(stderr, "\t<max procesos> : numero maximo de procesos (3 - 33)\n");
	fprintf(stderr, "\tvelocidad: \"normal\" o vacio para velocidad normal\n");
	fprintf(stderr, "\t           \"veloz\" para velocidad rapida\n");
}


int main(int argc, char *argv[]){

	//Control de argumentos
	if (argc < 2){
		printHelp();
		exit(-1);
	}
	
	//Maximo de procesos
	int maxproc = atoi(argv[1]);
	if (maxproc < 3 || maxproc > 33) printHelp();
	
	//Velocidad
	int speed = 0;
	if (argc >= 3){
		if (!strcmp(argv[2], "veloz")){
			speed = 1;
		}
		else if (!strcmp(argv[2], "normal")){
			speed = 0;
		}
		else {
			printHelp();
			exit(-1);
		}
	}
	else {
		speed = 0;
	}
	
	//Directrices iniciales
	char buf[32];    //Buffer para impresion de texto.
	
	sprintf(buf, "\nMaximo de procesos: %d\n", maxproc);
	write(1, buf, strlen(buf));
	
	sprintf(buf, "V(%d)\n", getpid());
	write(1, buf, strlen(buf));
	
	//Registro de señales
	signal(SIGINT, sh);
	signal(SIGCHLD, sh);
	signal(SIGTERM, sh);
	signal(SIGUSR1, sh);
	signal(SIGALRM, sh);
	
	//Estructura de file lock
	struct flock fl;
	fl.l_type = F_WRLCK;    
	fl.l_whence = SEEK_SET;    
	fl.l_start = 0;            
	fl.l_len = 0;            
	fl.l_pid = getpid();    
	
	//Gestión de lista
	childs = createList();
	if (childs == NULL){
		fprintf(stderr, "List error.\n");
		exit(-1);
	}
	
	//Crear archivo de control
	int fd = open("nproc", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd == -1){
		perror("File error");
		exit(-1);
	}
	
	
	int nproc = 1;         //Variable numerica, numero de procesos activos.
	write(fd, "1", 2);
	close(fd);
	
	//Inicializar RNG (Generador de numeros aleatorios) con una nueva semilla.
	srand(time(NULL));
	
	//Variables auxiliares
	int sleeptime;    //Tiempo aleatorio que dormir en modo normal.
	int fr, wr;        //resultado del fork (fork result) o escritura.        
	int isroot = 1;    //true (1) si el proceso es el proceso raiz. false (0) si no
	int st;            //Tiempo que falta para dormir el tiempo estipulado.
	int create;        //true (1) si el proceso va a crear un hijo, false si no.
	int sigint = 0;    //Indica terminacion por una interrupcion (SIGINT).
	
	//Crear file descriptor
	fd = open("nproc", O_RDWR);
	if (fd == -1){
		perror("File error");
		exit(-1);
	}
	
	//Auxiliar para recorrer la lista.
	ll_node *aux;    
		
	//Bucle principal
	while(1){
		

		//Control de estado
		switch(state){
			/*
			case STATE_WAIT:
				//fprintf(stderr, "%d Entered WAIT", getpid());                
				while (childs->first != NULL) pause();
				state = STATE_END;
				break;
			*/
			case STATE_RUNNING:
				break;
				
			case STATE_INTERRUPT:
				sigint = 1;
				aux = childs->first;
				while (aux != NULL){
					kill(aux->pid, 15);
					aux = aux->next;
				}
				
				//Esperar hasta que mueran todos.
				while (1){
					pid_t done = wait(NULL);
					if (done == -1) break;
				}
				
				state = STATE_END;
				break;
			
			case STATE_TERMINATE:
				
				aux = childs->first;
				while (aux != NULL){
					kill(aux->pid, 15);
					aux = aux->next;
				}
				
				while (1){
					pid_t done = wait(NULL);
					if (done == -1) break;
				}
				
				state = STATE_END;
				break;
			
			case STATE_END:
				//fprintf(stderr, "%d Entered END", getpid());
				if (!sigint){
					//Si no es interrupcion, actualizar el archivo de control.
				
					fl.l_type = F_WRLCK;
					if (fcntl(fd, F_SETLKW, &fl) == -1) continue;
					lseek(fd, 0, SEEK_SET);
					wr = read(fd, buf, 3);
					nproc = atoi(buf);
					if (wr == -1){
						perror("File error");
						fl.l_type = F_UNLCK;
						fcntl(fd, F_SETLK, &fl);
						exit(-1);
					}
					lseek(fd, 0, SEEK_SET);
					sprintf(buf, "%d", --nproc);
					wr = write(fd, buf, strlen(buf));
					if (wr == -1){
						perror("File error");
						fl.l_type = F_UNLCK;
						fcntl(fd, F_SETLK, &fl);
						exit(-1);
					}
					fl.l_type = F_UNLCK;
					fcntl(fd, F_SETLK, &fl);
				}
				
				sprintf(buf, "M(%d)\n", getpid());
				write(1, buf, strlen(buf));
				
				//Liberar recursos.
				/*
				Se podria (y quizas deberia, por buen habito),  liberar la
				memoria dinamica de la lista aqui, pero ya que el proceso muere
				inmediatamente despues, la memoria es automaticamente liberada.
				*/
				close(fd);
				fflush(stdout);
				
				if (isroot){
					sprintf(buf, "Programa acabado correctamente");
					write(1, buf, strlen(buf));
				}
				
				exit(0);
				
			
		}
		
		
		//Directriz #1 - Pausa aleatoria
		
		sleeptime = rand()%5 + 1;
		
		
		//Si un proceso dormido recibe cualquier señal, es despertado y sleep()
		//devolvera el tiempo que faltaba por dormir. De este modo, si el
		//proceso despierta antes de tiempo, st sera =/= 0 y se dormira el
		//tiempo restante.
		if (state == STATE_RUNNING && !speed){
			st = sleep(sleeptime);
			while (st && state == STATE_RUNNING) st = sleep(st);
		}
		
		//Directriz #2 - Generación de filiales o autodestrucción
		
		if (!isroot) create = rand()%2;
		else create = 1;
		
		if (state != STATE_RUNNING) continue; //Failsafe de salida
		/*
		Esto evita realizar una ejecucion completa del bucle con su bloqueo de
		archivo correspondiente si una señal ha sacado al proceso de su estado
		normal.
		*/
		
		//Generación de filial
		if (create){
		
			//Establecer lock e incrementar archivo de control.
			fl.l_type = F_WRLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1) continue;
			lseek(fd, 0, SEEK_SET);
			wr = read(fd, buf, 3);
			if (wr == -1){
				perror("File error");
				fl.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &fl);
				continue;
			}
			nproc = atoi(buf);
			if (nproc == maxproc){
				fl.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &fl);
				continue;
			}
			lseek(fd, 0, SEEK_SET);
			sprintf(buf, "%d", ++nproc);
			wr = write(fd, buf, strlen(buf));
			if (wr == -1){
				perror("File error");
				fl.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &fl);
				continue;
			}
			
		
			//Generar filial
			fr = fork();
			switch (fr){
				case -1:
				
					perror("Fork Error");
					//Desbloquear archivo.
					fl.l_type = F_UNLCK;    
					fcntl(fd, F_SETLK, &fl);
					break;
					
				case 0: //Hijo
					
					//Re-registrar senyales, los hijos no heredan las del padre.
					signal(SIGCHLD, sh);
					signal(SIGTERM, sh);
					signal(SIGUSR1, sh);
					signal(SIGALRM, sh);
					signal(SIGINT, sh);
					
					//Indicar que ya no es raiz.
					isroot = 0;
					fl.l_pid = getpid();
					
					//Vaciar lista (Se hereda la del padre).
					while(childs->first != NULL){
						removeChild(childs, childs->first->pid);
					}
					
					//Crear nuevo fdesc (Evita problemas de concurrencia con 
					//lseek)
					close(fd);
					fd = open("nproc", O_RDWR);
					if (fd == -1){
						perror("File error");
						exit(-1);
					}
					
					//Directriz de nacimiento
					sprintf(buf, "V(%d)\n", getpid());
					write(1, buf, strlen(buf));
					
					//Reactivar padre
					kill(getppid(), SIGUSR1);
					
					break;
					
				default: //Padre
					
					addChild(childs, fr);
					
					//Detener padre y esperar a la creación.
					/*
					alarm(2) es una precaucion contra la eventualidad de una
					condicion de carrera entre el padre y el hijo por la cual el
					hijo envie SIGUSR1 (que despertaria al padre) antes de que
					el padre use pause(). A los 2 segundos, el sistema enviara
					SIGALRM al padre, despertandolo si aun esta en pausa.
					*/
					if (state == STATE_RUNNING) state = STATE_WAIT_CHILD;
					alarm(2);
					pause();
					
					//Desbloquear archivo.
					fl.l_type = F_UNLCK;    
					fcntl(fd, F_SETLK, &fl);
									
			}
			
		}
		//Autodestrucción recursiva
		else {
			state = STATE_TERMINATE;
		}    
	}    
}

//Código auxiliar de listas enlazadas
int addChild(ll_childs* list, int pid){
	if (list == NULL) return 0;
	ll_node *node = (ll_node*) malloc(sizeof(ll_node));
	if (node == NULL) return 0;
	node->pid = pid;
	node->next = NULL;
	if (list->first == NULL && list->last == NULL){
		list->first = node; 
		list->last = node;
	}
	else{
		list->last->next = node;
		list->last = node;
	}
	return 1;
}

void removeChild(ll_childs* list, int pid){
	if (list == NULL) return;
	ll_node *aux = list->first;
	ll_node *prev = NULL;
	while (aux != NULL){
		if (aux->pid == pid){
			if (prev == NULL) list->first = aux->next;
			else prev->next = aux->next;
			if (aux->next == NULL) {
				list->last = prev;
			}
			free(aux);
			return;
		}
		prev = aux;
		aux = aux->next;
	}
}

ll_childs* createList(){
	ll_childs *childs = (ll_childs*) malloc(sizeof(ll_childs));
	if (childs == NULL) return NULL;
	childs->first = NULL;
	childs->last = NULL;
	return childs;
}

void printList(ll_childs* childs){
	if (childs == NULL) return;
	ll_node *aux = childs->first;
	while (aux != NULL){
		fprintf(stderr, "%d\n", aux->pid);
		aux = aux->next;
	}
}