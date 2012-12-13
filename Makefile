C_FILES=$(wildcard *.c)
O_FILES=$(C_FILES:.c=.o)
CC= gcc
GL_LIBS= -lglut -lGLU
PROG= game_of_life



.PHONY: all
all: $(PROG)
	
$(PROG): $(O_FILES)
	$(CC) -o $@ $^ $(GL_LIBS)	

%.o: %.c
	$(CC) -c $^

clean:
	rm -rf $(O_FILES) $(PROG)
