CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -o2 -g


SRC = src/main.cc src/Particle.cc src/World.cc src/Game.cc
OBJ = $(SRC:.cc=.o)
EXEC = sim


LDLIBS = -lcurl

all: $(EXEC)

$(EXEC): $(OBJ)
		$(CXX) $(OBJ) -o $(EXEC) $(LDLIBS)

src/%.o: src/%.cc
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f src/*.o (EXEC)
