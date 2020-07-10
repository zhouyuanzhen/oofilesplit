APP="oofilesplit"
CC?= clang

compile:
	$(CC) main.c -O2 -o $(APP)

test: compile
	./$(APP) -h

clean:
	rm $(APP)
