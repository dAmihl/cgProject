CC = g++
LD = gcc

OBJ = MerryGoRound.o LoadShader.o Matrix.o StringExtra.o OBJParser.o List.o
TARGET = MerryGoRound
#-Wextra
CFLAGS = -g -Wall 
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
# 
clean: 
	rm -f $(BUILD_DIR)/*.o *.o $(TARGET) 
# 
.PHONY: clean

# Dependencies 
$(TARGET): $(BUILD_DIR)/LoadShader.o $(BUILD_DIR)/Matrix.o $(BUILD_DIR)/StringExtra.o $(BUILD_DIR)/OBJParser.o  $(BUILD_DIR)/List.o | $(BUILD_DIR)


# Rules
# \
MerryGoRound: $(OBJ) \
	 $(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

# \
clean: \
	rm -f *.o MerryGoRound

# \
.PHONY: all clean
