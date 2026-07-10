# ============================================================
# Starlight Hotel Management System - Makefile
# ============================================================
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS  =

SRC = src/main.cpp \
      src/Utility.cpp \
      src/Room.cpp \
      src/Customer.cpp \
      src/Staff.cpp \
      src/FileManager.cpp \
      src/HotelManagementSystem.cpp

TARGET = HotelMS

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
