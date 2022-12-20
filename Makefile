CFILES = main.c
TARGET = cfreinds

CC = gcc
CFLAGS = -lGL -lGLU -lglut

all: $(TARGET)

$(TARGET): $(CFILES)
	$(CC) $(CFILES) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
