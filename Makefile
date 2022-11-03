CXX = g++
CXXFLAGS =-std=c++11 -Wall -pedantic
LDFLAGS =  -lncurses 
EXEC = bnoc

SRC = main.cpp $(wildcard Sources/*.cpp)
OBJ = $(SRC:.cpp = .o)

all: $(EXEC)

bnoc : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -f *~ *.o

mrproper: clean
	rm -rf $(EXEC)
