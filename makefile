
COMPILER = cc
SRC = src/main.c
OUT = builded
OUT_DEBUG = debug_build

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

OUT = builded
all: $(OUT)
debug_all: $(OUT_DEBUG)
$(OUT_DEBUG): $(OBJ)
	$(COMPILER) -g $(OBJ) -o $(OUT_DEBUG) $(LIBS)

$(OUT): $(OBJ)
	$(COMPILER) $(OBJ) -o $(OUT) $(LIBS)
%.o: %.c
	$(COMPILER) -c $< -o $@
run_base: all
	./$(OUT) examples/base.nim

debug: debug_all
	lldb ./$(OUT_DEBUG) examples/base.nim
run: all
	./$(OUT)
clean:
	rm -f $(OUT)
	rm -f $(OBJ)