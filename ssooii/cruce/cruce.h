#define NEGRO    0
#define ROJO     1
#define VERDE    2
#define AMARILLO 3
#define AZUL     4
#define MAGENTA  5
#define CYAN     6
#define BLANCO   7

#define SEM_C1 0
#define SEM_C2 1
#define SEM_P1 2
#define SEM_P2 3

#define COCHE 0
#define PEAToN 1

struct posiciOn {int x,y;};

void pon_error(char *mensaje);

int CRUCE_inicio(int ret, int maxProcs, int semAforos, char *zona);
int CRUCE_fin(void);
int CRUCE_pon_semAforo(int sem, int color);
int CRUCE_nuevo_proceso(void);
struct posiciOn CRUCE_inicio_coche(void);
struct posiciOn CRUCE_avanzar_coche(struct posiciOn sgte);
int CRUCE_fin_coche(void);
struct posiciOn CRUCE_inicio_peatOn(void);
struct posiciOn CRUCE_inicio_peatOn_ext(struct posiciOn *posNacimiento);
struct posiciOn CRUCE_avanzar_peatOn(struct posiciOn sgte);
int CRUCE_fin_peatOn(void);
int pausa();
int pausa_coche();

