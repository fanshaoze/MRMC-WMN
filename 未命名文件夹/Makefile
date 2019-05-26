meshclient:main.o inform_get.o command.o util.o send.o receive.o
	gcc main.o inform_get.o command.o util.o send.o receive.o -o meshclient
main.o:main.c
	gcc -c main.c -o main.o
inform_get.o:inform_get.c
	gcc -c inform_get.c -o inform_get.o
command.o:command.c
	gcc -c command.c -o command.o
util.o:util.c
	gcc -c util.c -o util.o
send.o:send.c
	gcc -c send.c -o send.o
receive.o:receive.c
	gcc -c receive.c -o receive.o
