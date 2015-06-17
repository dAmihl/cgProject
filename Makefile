CC = g++
LD = gcc

OBJ = MerryGoRound.o LoadShader.o LoadTexture.o CameraMovement.o DeltaTime.o
TARGET = MerryGoRound
#-Wextra
CFLAGS = -g -Wall -std=c++0x
LDLIBS = -lm -lglut -lGLEW -lGL
INCLUDES = -Isource
SRC_DIR = source
BUILD_DIR = build
VPATH = source

# new Rules 
all: $(TARGET)
# 
$(TARGET).o: $(TARGET).cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@
# 
$(BUILD_DIR)/%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@
# 
clean: 
	rm -f $(BUILD_DIR)/*.o *.o $(TARGET) 
# 
.PHONY: clean

# Dependencies 
$(TARGET): $(BUILD_DIR)/LoadTexture.o $(BUILD_DIR)/LoadShader.o $(BUILD_DIR)/CameraMovement.o $(BUILD_DIR)/DeltaTime.o | $(BUILD_DIR)


# Rules
# \
MerryGoRound: $(OBJ) \
	 $(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

# \
clean: \
	rm -f *.o MerryGoRound

# \
.PHONY: all clean
