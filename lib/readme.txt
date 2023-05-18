g++ -c main.cpp -o main.o

g++ main.o my_math.o -o a.out
./a.out
0.5

https://domiyanyue.medium.com/c-development-tutorial-4-static-and-dynamic-libraries-7b537656163e

ar cr libmy_math.a my_math.o

g++ -c main.cpp -o main.o
g++ -c my_math.cpp -o my_math.o
g++ main.o my_math.o -o a.out


