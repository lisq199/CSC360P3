CC = gcc
CFLAGS = -Wall -ansi
DEPS = bootsector.h fattable.h p3utils.h rootdir.h
OBJ = bootsector.o fattable.o p3utils.o rootdir.o
EXENAME = diskinfo disklist diskget diskput

EXENAME:
	make $(EXENAME)

diskinfo: diskinfo.o $(OBJ)
	$(CC) -o $@ $^

disklist: disklist.o $(OBJ)
	$(CC) -o $@ $^

diskget: diskget.o $(OBJ)
	$(CC) -o $@ $^

diskput: diskput.o $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(EXENAME) $(OBJ) diskinfo.o disklist.o diskget.o diskput.o
