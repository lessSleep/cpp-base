g++ -c my_math.cpp -o my_math.o
ar cr libmy_math.a my_math.o
g++ -c main.cpp -o main.o
g++ main.o libmy_math.a -o a.out


#g++ main.o -L. -lmy_math -o a.out