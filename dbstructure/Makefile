FLAGS = -lmysqlclient -DBIG_JOINS=1 -g -DNDEBUG -Wall -pedantic -std=c++11
CFILES = $(wildcard lib/*/*.cpp)
OBJECTS = $(patsubst lib/%.cpp, bin/%.o, $(CFILES))
all: dbstructure
	@echo "" > /dev/null
dbstructure: dbstructure.cpp $(OBJECTS)
	g++ dbstructure.cpp $(OBJECTS) -o dbstructure $(FLAGS)
bin/%.o: lib/%.cpp lib/%.hpp
	@mkdir -p bin
	@mkdir -p bin/$*
	@rmdir bin/$*
	g++ $(FLAGS) -c $< -o $@
clean:
	rm dbstructure
	rm bin/**/*.o
