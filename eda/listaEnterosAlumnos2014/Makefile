CC   = gcc -D__DEBUG__

OBJ  = pruebaLista.o tipoLista.o tipoElemento.o
LINKOBJ  = pruebaLista.o tipoLista.o tipoElemento.o
LIBS =  -pg  -g3 
INCS =   
CXXINCS =  
BIN  = pruebaLista
CXXFLAGS = $(CXXINCS)   -fverbose-asm -O3 -pg -g3
CFLAGS = $(INCS)   -fverbose-asm -O3 -pg -g3
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before pruebaLista all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "pruebaLista" $(LIBS)

pruebaLista.o: pruebaLista.c tipoLista.h tipoElemento.h
	$(CC) -c pruebaLista.c -o pruebaLista.o $(CFLAGS)

tipoLista.o: tipoLista.c tipoLista.h tipoElemento.h
	$(CC) -c tipoLista.c -o tipoLista.o $(CFLAGS)
	
tipoElemento.o: tipoElemento.c tipoElemento.h
	$(CC) -c tipoElemento.c -o tipoElemento.o $(CFLAGS)
	

	
	