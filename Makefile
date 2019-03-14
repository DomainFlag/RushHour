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
EXECS := $(wildcard ./*.exe)
SOURCES = $(filter-out ./obj/main.o, $(OBJS))

.PHONY: clean help print

all: clean $(TARGET)

check: clean $(ITEM)

$(ITEM): ./tests/test_$(item).cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lSDL2 -lSDL2_ttf
	$(eval TARGET = $(ITEM))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lSDL2 -lSDL2_ttf

$(OBJDIR)%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.h:
	@:

clean:
	rm -rf $(OBJS) $(EXECS)

print:
	ls ./tests

help:
	@echo "Running main program:"
	@echo "  none            - Build and execute main program"
	@echo "Running test items:"
	@echo "  print           - Show all available tests"
	@echo "  check item=n    - Build and execute nth test from the available tests"
	@echo "Cleaning targets:"
	@echo "  clean           - Remove all generated files"
