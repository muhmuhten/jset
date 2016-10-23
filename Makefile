it:	all

PROGS	:=	jat jrm jset
CFLAGS	+=	-Wall -pedantic -s -ljail

all:	$(PROGS)
clean:
	rm -f $(PROGS)
