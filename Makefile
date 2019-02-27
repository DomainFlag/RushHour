CXX = g++
CXXFLAGS = -g -Wall --std=c++11
TARGET = main

vpath %.cpp ./src
vpath %.h ./include
vpath %.o ./obj

OBJS := $(patsubst ./src/%.cpp, ./obj/%.o, $(wildcard ./src/*.cpp))
OBJS := $(filter-out ./obj/Test.o, $(OBJS))
OBJDIR = ./obj/

.PHONY: clean run

all: clean $(TARGET) run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.h:
	@:

clean:
	rm -rf $(OBJS) $(TARGET)

run:
	./$(TARGET)
