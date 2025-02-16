# Makefile

# Настройки компилятора
CC = gcc
NASM = nasm
CFLAGS = -Wall -Wextra
NASMFLAGS = -f elf

# Имена файлов
C_SOURCE = add.c
ASM_SOURCE = example.asm
C_OBJECT = $(C_SOURCE:.c=.o)
ASM_OBJECT = $(ASM_SOURCE:.asm=.o)
EXECUTABLE = program

# Цель по умолчанию
all: $(EXECUTABLE)

# Компиляция С-файла в объектный файл
$(C_OBJECT): $(C_SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция NASM-файла в объектный файл
$(ASM_OBJECT): $(ASM_SOURCE)
	$(NASM) $(NASMFLAGS) $< -o $@

# Сборка исполняемого файла
$(EXECUTABLE): $(C_OBJECT) $(ASM_OBJECT)
	$(CC) $(C_OBJECT) $(ASM_OBJECT) -o $(EXECUTABLE)

# Очистка временных файлов
clean:
	rm -f $(C_OBJECT) $(ASM_OBJECT) $(EXECUTABLE)