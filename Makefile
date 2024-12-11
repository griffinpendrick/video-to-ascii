# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -arch arm64

# Paths
RAYLIB_DIR = ./raylib
OPENCV_DIR = /opt/homebrew/Cellar/opencv/4.10.0_12

INCLUDE = -I$(RAYLIB_DIR)/include -I$(OPENCV_DIR)/include/opencv4
LDFLAGS = -L$(RAYLIB_DIR)/lib -L$(OPENCV_DIR)/lib -Wl,-rpath,$(OPENCV_DIR)/lib
LIBS = -lraylib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio \
       -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL

# Target executable
TARGET = main

# Source files
SRCS = main.cpp

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(INCLUDE) $(LDFLAGS) $(LIBS)

# Clean target
clean:
	rm -f $(TARGET)

