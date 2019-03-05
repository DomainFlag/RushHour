CXX = g++
CXXFLAGS = -g -Wall --std=c++11
TARGET = main
ITEM = item
OBJDIR = ./obj/

vpath %.cpp ./src ./tests
vpath %.h ./include
vpath %.o ./obj

TESTS := $(patsubst ./tests/%.cpp, ./obj/%.o, $(wildcard ./tests/*.cpp))
OBJS := $(patsubst ./src/%.cpp, ./obj/%.o, $(wildcard ./src/*.cpp))
SOURCES = $(filter-out ./obj/main.o, $(OBJS))

.PHONY: clean run help print

all: clean $(TARGET) run

check: clean $(ITEM) run

$(ITEM): ./tests/test$(item).cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@
	$(eval TARGET = $(ITEM))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.h:
	@:

clean:
	rm -rf $(OBJS) $(TARGET)

print:
	ls ./tests

run:
	./$(TARGET)

help:
	@echo "Running main program:"
	@echo "  none            - Build and execute main program"
	@echo "Running test items:"
	@echo "  print           - Show all available tests"
	@echo "  check item=n    - Build and execute nth test from the available tests"
	@echo "Cleaning targets:"
	@echo "  clean           - Remove all generated files"
