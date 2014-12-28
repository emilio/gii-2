FLAGS = -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing   -g -DNDEBUG
dbstructure: dbstructure.cpp
	g++ dbstructure.cpp -o dbstructure $(FLAGS)
clean:
	rm dbstructure
