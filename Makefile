CC = gcc
TARGET = hsc.exe
LUA_DIR = vendor/lua/src
PDCURSES_DIR = vendor/PDCurses
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))
CFLAGS = -I$(PDCURSES_DIR) -I$(LUA_DIR) -Isrc/include -g
LIBS = $(PDCURSES_DIR)/wincon/pdcurses.a $(LUA_DIR)/liblua.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo hs linked

obj/%.o: src/%.c
	@if not exist obj mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist obj rmdir /s /q obj
	@if exist $(TARGET) del /q /f $(TARGET)
	@echo cleaned

run: all
	./$(TARGET)