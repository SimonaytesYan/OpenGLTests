OPENGL_FLAG   = -lGL -lGLU
X_FLAG		  = -lX11
COMPILE_FLAGS = -O2 $(OPENGL_FLAG) $(X_FLAG)
CPP   		  = g++ $(COMPILE_FLAGS)

OBJ = Obj/
SRC	= Src/
EXE = Exe/run

GREEN_COLOR   = '\033[0;32m'
DEFAULT_COLOR = '\033[0m'

all:
	$(CPP) $(SRC)main.cpp -o $(EXE)
	@echo  $(GREEN_COLOR) Compilation ends successfully! $(DEFAULT_COLOR)

#$(OBJ)NotInt.o: $(SRC)NotInt/NotInt.cpp $(SRC)NotInt/NotInt.h
#	$(CPP) -c $(SRC)NotInt/NotInt.cpp -o $(OBJ)NotInt.o

prepare:
	mkdir Exe/
	mkdir $(OBJ)

run:  all
	./$(EXE)

clean:
	-cd $(OBJ); rm *
	-rm $(EXE)