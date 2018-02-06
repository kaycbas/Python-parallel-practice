#------------------------------------------------------------------------------

CFLAGS= -fopenmp
CC=gcc

#------------------------------------------------------------------------------

all: p1_serial p1_for p1_section p2_serial p2_omp 

### 

p1_serial: p1_serial.c
	$(CC) -o p1_serial p1_serial.c $(CFLAGS)

p1_for: p1_for.c
	$(CC) -o p1_for p1_for.c $(CFLAGS)

p1_section: p1_section.c
	$(CC) -o p1_section p1_section.c $(CFLAGS)

p2_serial: p2_serial.c
	$(CC) -o p2_serial p2_serial.c $(CFLAGS)

p2_omp: p2_omp.c
	$(CC) -o p2_omp p2_omp.c $(CFLAGS)


###


