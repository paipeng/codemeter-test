CC=gcc

TARGET=libtest.so

#LIBS = -lwibucmhip
#LIBS += -lwibucmhipc
LIBS = -lwibucm

LIB_PATH = -L./
#LIB_PATH = -L/usr/lib/aarch64-linux-gnu

SRCS = test.c test_codemeter.c


OBJ = $(patsubst %.c, $(INC_PATH)%.o,$(SRCS))

     
%.o: %.c
	$(CC) -fPIC -c -o $@ $(patsubst %.o, %.c,$@)  $(CFLAGS)

main:all

so:	$(OBJ)
	$(CC) -fPIC $^ -shared -o $(TARGET) $(CFLAGS) $(INC_LIB_PATH) $(LIBS)
	rm -f *.o
exe:
	$(CC) tester.c -ltest $(LIB_PATH) -o test_so
