#define NEGRO    0
#define ROJO     1
#define VERDE    2
#define AMARILLO 3
#define AZUL     4
#define MAGENTA  5
#define CYAN     6
#define BLANCO   7

void pon_error(char *mensaje);
int PIST_inicio(unsigned int nPistoleros,
                int ret, int semAforos, char *zona, int semilla);
int PIST_nuevoPistolero(char pist);
char PIST_vIctima(void);
int PIST_disparar(char pist);
int PIST_morirme(void);
int PIST_fin(void);
