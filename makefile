CXX = g++
CXXFLAGS = -g -Wall
LIBS = -lncurses

CPPFILES = src/main.cpp src/game.cpp src/functions.cpp src/color.cpp src/ui.cpp
OBJFILES = $(CPPFILES:.cpp=.o)

TARGET = soks

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJFILES)
	$(RM) $(TARGET)
