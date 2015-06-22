CC = g++
LD = gcc

OBJ = MerryGoRound.o LoadShader.o LoadTexture.o CameraMovement.o DeltaTime.o image_DXT.o SOIL/image_helper.o SOIL/SOIL.o
TARGET = MerryGoRound
#-Wextra
CPPFLAGS = -g -Wall -std=c++0x 
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm -lglut -lGLEW -lGL
INCLUDES = -Isource -ISOIL
SRC_DIR = source
BUILD_DIR = build
VPATH = source

# new Rules 
all: $(TARGET)
# 
$(TARGET).o: $(TARGET).cpp 
	$(CC) $(CPPFLAGS) $(INCLUDES)  -c $^ -o $@
# 
$(BUILD_DIR)/%.o: %.c 
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $^ -o $@
# 
clean: 
	rm -f $(BUILD_DIR)/*.o *.o $(TARGET) 
# 
.PHONY: clean

# Dependencies 
$(TARGET): $(BUILD_DIR)/LoadTexture.o $(BUILD_DIR)/LoadShader.o $(BUILD_DIR)/CameraMovement.o $(BUILD_DIR)/DeltaTime.o $(BUILD_DIR)/image_DXT.o $(BUILD_DIR)/image_helper.o $(BUILD_DIR)/SOIL.o $(BUILD_DIR)/stb_image_aug.o | $(BUILD_DIR)
