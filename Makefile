APP="oofilesplit"
CC?= gcc

compile:
	$(CC) main.c -o $(APP)

test:
	./$(APP) -h

clean:
	rm $(APP)
