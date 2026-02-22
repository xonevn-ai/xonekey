CXX = g++
CXXFLAGS = -std=c++11 -Wall -DLINUX -I.

SRCS = Engine.cpp Vietnamese.cpp ConvertTool.cpp Macro.cpp SmartSwitchKey.cpp test_engine.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = test_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
