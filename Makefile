PROG    =       free
CFLAGS  +=      -Werror -Wall -Wextra
SRCS    =       free.c
LDFLAGS +=      
NO_MAN  =       sorry
CLEANFILES = $(PROGS) *~ $(OBJECTS)
.include <bsd.prog.mk>
