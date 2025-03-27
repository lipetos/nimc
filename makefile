
COMPILER = cc
SRC = src/main.c
OUT = builded

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

OUT = builded
all: $(OUT)
$(OUT): $(OBJ)
	$(COMPILER) $(OBJ) -o $(OUT) $(LIBS)
%.o: %.c
	$(COMPILER) -c $< -o $@
run_base: all
	./$(OUT) examples/base.nim
run: all
	./$(OUT)
clean:
	rm -f $(OUT)
	rm -f $(OBJ)