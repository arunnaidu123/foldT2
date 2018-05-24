CC=gcc -g
CFLAGS=-lm
DEPS = tempo2.h header.h fold.h tempo2pred.h sigproc.h tempo2Util.h \
	tempo2pred_int.h polyco.h epn.h version.h T2accel.h config.h \

OBJ = foldT2InPhase.o file_exists.o cheby2d.o t1polyco.o \
	read_polyco.o tempo2pred.o error_message.o \
    strings_equal.o read_header.o t1polyco.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

foldT2InPhase: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean: 
	rm -rf *.o
	rm -rf *~
