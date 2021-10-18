OUT = build/raytrace
CFLAGS = -O2 -Wall -Wpedantic
LDFLAGS = -lm

OBJ = $(shell find -name *.c | sed "s/src/work/g" | sed "s/\.c/.o/g")

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(OUT)

work/%.o: src/%.c
	$(CC) $< $(CFLAGS) -c -o $@
