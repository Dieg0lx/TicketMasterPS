# ==============================
# Makefile - TicketMaster (raíz)
# ==============================

# Compilador y banderas
CC      = gcc
CFLAGS  = -g -Iinclude -Isrc -Isrc/BackEnd -Isrc/FrontEnd
LDFLAGS = -Llib
LDLIBS  = -lraylib -lopengl32 -lgdi32 -lwinmm
# Si enlazas raylib estática y te faltan símbolos, puedes añadir:
# LDLIBS += -lkernel32 -lshell32

# Carpetas
SRC_DIR      = src
BACKEND_DIR  = $(SRC_DIR)/BackEnd
FRONTEND_DIR = $(SRC_DIR)/FrontEnd
TEST_DIR     = $(SRC_DIR)/Tests
BUILD_DIR    = build

# Fuentes
SRC_MAIN     = $(SRC_DIR)/TicketMaster.c
SRC_BACKEND  = $(wildcard $(BACKEND_DIR)/*.c)
SRC_FRONTEND = $(wildcard $(FRONTEND_DIR)/*.c)
SRC_TESTS    = $(wildcard $(TEST_DIR)/*.c)

# Objetos (se generan en build/ preservando subcarpetas)
OBJS_MAIN  = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_MAIN)) \
             $(patsubst $(BACKEND_DIR)/%.c,$(BUILD_DIR)/BackEnd/%.o,$(SRC_BACKEND)) \
             $(patsubst $(FRONTEND_DIR)/%.c,$(BUILD_DIR)/FrontEnd/%.o,$(SRC_FRONTEND))


OBJS_TESTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/Tests/%.o,$(SRC_TESTS)) \
			 $(patsubst $(BACKEND_DIR)/%.c,$(BUILD_DIR)/BackEnd/%.o,$(SRC_BACKEND)) \
             $(patsubst $(FRONTEND_DIR)/%.c,$(BUILD_DIR)/FrontEnd/%.o,$(SRC_FRONTEND))


# Ejecutables
TARGET      = TicketMaster.exe
TEST_TARGET = test_menu.exe

.PHONY: all test clean info run

# ==============================
# Reglas principales
# ==============================

all: $(TARGET)

$(TARGET): $(OBJS_MAIN) | $(BUILD_DIRS)
		$(CC) $(OBJS_MAIN) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

# ==============================
# Compilación de pruebas
# ==============================

test: $(TEST_TARGET)

$(TEST_TARGET): $(OBJS_TESTS) | $(BUILD_DIRS)
		$(CC) $(OBJS_TESTS) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
		@echo ---------------------------------------
		@echo Ejecutando pruebas unitarias...
		@echo ---------------------------------------
		./$(TEST_TARGET)

# ==============================
# Reglas .c -> .o (hacia build/)
# ==============================

# Crear carpetas build y subcarpetas (compatible con CMD/PowerShell)
BUILD_DIRS := $(BUILD_DIR) $(BUILD_DIR)/FrontEnd $(BUILD_DIR)/BackEnd $(BUILD_DIR)/Tests

$(BUILD_DIRS):
	if not exist "$@" mkdir "$@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/FrontEnd/%.o: $(FRONTEND_DIR)/%.c | $(BUILD_DIR)/FrontEnd
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/BackEnd/%.o: $(BACKEND_DIR)/%.c | $(BUILD_DIR)/BackEnd
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Tests/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)/Tests
	$(CC) $(CFLAGS) -c $< -o $@

# ==============================
# Utilidades
# ==============================

run: $(TARGET)
	./$(TARGET)

clean:
	@del /Q "$(TARGET)" "$(TEST_TARGET)" 2> NUL || true
	@rmdir /S /Q "$(BUILD_DIR)" 2> NUL || true
	@echo "Archivos compilados eliminados."

info:
	@echo "=== TicketMaster Makefile (root) ==="
	@echo "Compilador: $(CC)"
	@echo "CFLAGS:     $(CFLAGS)"
	@echo "LDFLAGS:    $(LDFLAGS)"
	@echo "LDLIBS:     $(LDLIBS)"