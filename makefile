CC = x86_64-w64-mingw32-gcc

SIZE = -Os -s
WARNINGS = -Wall -Wextra -Wpedantic
FREESTANDING = -nostdlib -ffreestanding -fno-stack-check -fno-stack-protector -mno-stack-arg-probe
SUBSYSTEM = -mwindows
LIBS = -lkernel32 -luser32 -lgdi32 -lopengl32
MISC = -fno-ident

CFLAGS = $(SIZE) $(WARNINGS) $(FREESTANDING) $(SUBSYSTEM) $(LIBS) $(MISC)

window: window.c
	$(CC) -o $@ $< $(CFLAGS)