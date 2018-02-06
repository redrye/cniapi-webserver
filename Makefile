.c.o:
	gcc -c -I. -o $@ $<

all: webserver

webserver.exe: webserver.o cnaiapi_init.o cname_to_comp.o readln.o send_eof.o await_contact.o
	gcc -o webserver webserver.o cnaiapi_init.o cname_to_comp.o \
        readln.o send_eof.o await_contact.o -l wsock32
