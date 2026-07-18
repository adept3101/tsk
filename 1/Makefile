all: main

main:
	gcc main.c -I../lib/include -L../lib -pthread -lstro -Wl,-rpath,'$$ORIGIN/../../lib' -o build/main
