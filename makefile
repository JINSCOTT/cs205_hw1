# Makefile for the C program
# ref:https://makefiletutorial.com/

CXX = g++
CFLAGS = -Wall -g
TARGET = hw1
OBJS = main.o al.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp al.h
	$(CXX) $(CFLAGS) -c main.cpp

al.o: al.cpp al.h
	$(CXX) $(CFLAGS) -c al.cpp

clean:
	rm -f $(TARGET) $(OBJS)