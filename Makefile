CC = gcc
CFLAGS = -O2 -g -Wall -DNDEBUG -DLOG_USE_COLOR -std=c99
LIBS = -lsqlite3 -lcriterion
OBJ = main.o
DEPS = log.o addbook.o fort.o
TEST = test.o
BIN = addbook

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

test: $(DEPS) $(TEST)
	sqlite3 test.db ".databases"
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
	./test; $(RM) test

docker_build:
	docker build --tag addbook .
	docker login
	docker tag addbook:latest smalinux/addbook
	docker push smalinux/addbook

clean:
	$(RM) *.o
	$(RM) addbook
	$(RM) *.orig
