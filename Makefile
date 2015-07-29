WARNINGS:=-Wall -Wextra
FILES:=main simulation params critter food CasfireSplash MovableView NeuralNet RandomGenerator SDL_gfxPrimitives SDLGui SDLGuiStyle SDLHelper
BINARY:=Life
LIN_CFLAGS:=$(WARNINGS) -O
LIN_LFLAGS:=-static-libgcc -static-libstdc++ `sdl-config --cflags` -lSDL -lSDL_ttf
WIN_CFLAGS:=$(WARNINGS) -O
WIN_LFLAGS:=-static-libgcc -static-libstdc++ -lmingw32 -lSDLmain -lSDL -lSDL_ttf -mwindows

####################

UNAME:=$(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS:=$(LIN_CFLAGS)
LFLAGS:=$(LIN_LFLAGS)
else
CFLAGS:=$(WIN_CFLAGS)
LFLAGS:=$(WIN_LFLAGS)
endif
OBJS:=$(patsubst %,obj/%.o,$(FILES))
.PHONY: all clean dirs
all: dirs $(OBJS)
ifeq ($(UNAME), Linux)
	@echo "Linking..."
	@g++ $(OBJS) $(LFLAGS) -o bin/$(BINARY)
else
	@echo "Compiling resources..."
	@windres src/resources.rc -O coff -o obj/resources.o
	@echo "Linking..."
	@g++ $(OBJS) obj/resources.o $(LFLAGS) -o bin/$(BINARY)
endif
	@echo "Done."
clean:
	@rm -rf *.o bin/$(BINARY) bin/$(BINARY).exe obj/
	@echo "Cleaned."
dirs: obj/ bin/
obj/:
	@echo "Creating obj/ directory"
	@mkdir -p obj/
bin/:
	@echo "Creating bin/ directory"
	@mkdir -p bin/
obj/%.o: src/%.cpp
	@echo "Compiling $<"
	@g++ -c $< $(CFLAGS) -o $@
obj/%.o: src/%.c
	@echo "Compiling $<"
	@gcc -c $< $(CFLAGS) -o $@
