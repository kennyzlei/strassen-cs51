#
# Makefile
#
# Computer Science 51
# Final Project
#


# compiler to use
CC = clang

# flags to pass compiler
CFLAGS = -ggdb -O0 -Qunused-arguments -std=c99 -Wall -Werror

# name for executable
# We want different executables
EXE = regular winograd strassen intregular intwinograd intstrassen

# space-separated list of header files
HDRS = matrix.h bignum.h
HDRS_WE = matrix.h int_bignums/bignum.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS =

# space-separated list of source files
SRCS = matrix.c bignum.c regularMult.c winograd.c strassen.c
SRCS_REG = matrix.c bignum.c regularMult.c 
SRCS_WIN = matrix.c bignum.c winograd.c
SRCS_STR = matrix.c bignum.c strassen.c
SRCS_WE =  matrix.c int_bignums/bignum.c regularMult.c winograd.c strassen.c
SRCS_REG_WE = matrix.c int_bignums/bignum.c regularMult.c 
SRCS_WIN_WE = matrix.c int_bignums/bignum.c winograd.c
SRCS_STR_WE = matrix.c int_bignums/bignum.c strassen.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)
OBJS_REG = $(SRCS_REG:.c=.o)
OBJS_WIN = $(SRCS_WIN:.c=.o)
OBJS_STR = $(SRCS_STR:.c=.o)
OBJS_WE = $(SRCS_WE:.c=.o)
OBJS_REG_WE = $(SRCS_REG_WE:.c=.o)
OBJS_WIN_WE = $(SRCS_WIN_WE:.c=.o)
OBJS_STR_WE = $(SRCS_STR_WE:.c=.o)

# targets
all : regular winograd strassen intregular intwinograd intstrassen
	
regular: $(OBJS_REG) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_REG) $(LIBS)

winograd: $(OBJS_WIN) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_WIN) $(LIBS)

strassen: $(OBJS_STR) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_STR) $(LIBS)

intregular: $(OBJS_REG_WE) $(HDRS_WE) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_REG_WE) $(LIBS)

intwinograd: $(OBJS_WIN_WE) $(HDRS_WE) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_WIN_WE) $(LIBS)

intstrassen: $(OBJS_STR_WE) $(HDRS_WE) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS_STR_WE) $(LIBS)
	
# dependencies 

# housekeeping
clean:
	rm -f core $(EXE) *.o int_bignums/*.o
