@echo off
gcc -c foo.c
gcc -c bar.c
gcc -c main.c
gcc -o helloworld.exe main.o foo.o bar.o
 
.\helloworld.exe
@pause
