##
## pos_server Makefile
##
CC		= gcc
LIBS		= -lpq
SRCS		= pos_server_main.c setup_listen.c pos_controller.c common_func.c kokyaku.c
HDRS            = pos.h
OBJS		= $(SRCS:.c=.o)
PROGRAM		= pos_server

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS) $(HDRS) 
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
		rm -f *.o *~ $(PROGRAM)

###
