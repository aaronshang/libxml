ARCH32=x86
ARCH64=x64
INC_DIR=./inc
SRC_DIR=./src
BIN_DIR=./lib
OBJ_DIR=./obj

BIN_DIR_X86=$(BIN_DIR)/$(ARCH32)
BIN_DIR_X64=$(BIN_DIR)/$(ARCH64)

OBJ_DIR_X86=$(OBJ_DIR)/$(ARCH32)
OBJ_DIR_X64=$(OBJ_DIR)/$(ARCH64)

TARGET_DIR_X86=$(BIN_DIR_X86)
TARGET_DIR_X64=$(BIN_DIR_X64)

TARGET_NAME=libxml.a
TARGET_NAME_D=libxml_d.a

TARGET_PATH_X86=$(TARGET_DIR_X86)/$(TARGET_NAME)
TARGET_PATH_X64=$(TARGET_DIR_X64)/$(TARGET_NAME)

TARGET_PATH_X86_D=$(TARGET_DIR_X86)/$(TARGET_NAME_D)
TARGET_PATH_X64_D=$(TARGET_DIR_X64)/$(TARGET_NAME_D)

all:\
	all_x86 all_x64\
	all_x86_d all_x64_d
	
create_dirs_x86:
	mkdir -p $(OBJ_DIR_X86)/
create_dirs_x64:
	mkdir -p $(OBJ_DIR_X64)/
	
clean_x86:
	rm -rf $(OBJ_DIR_X86)/*.o
mrproper_x86:
	rm -rf $(TARGET_PATH_X86)
clean_x64:
	rm -rf $(OBJ_DIR_X64)/*.o
mrproper_x64:
	rm -rf $(TARGET_PATH_X64)
	
clean_x86_d:
	rm -rf $(OBJ_DIR_X86)/*_d.o
mrproper_x86_d:
	rm -rf $(TARGET_PATH_X86_D)
clean_x64_d:
	rm -rf $(OBJ_DIR_X64)/*_d.o
mrproper_x64_d:
	rm -rf $(TARGET_PATH_X64_D)
	
.PHONY:\
	create_dirs_x86 mrproper_x86 mrproper_x64 clean_x86 clean_x64\
	create_dirs_x64 mrproper_x86_d mrproper_x64_d clean_x86_d clean_x64_d

CC=gcc
AR=ar rcs
DEFINES=-D NDEBUG
DEFINES_D=

CFLAGS_X86=-Wall -Werror -Wextra -pedantic -ansi -m32 $(DEFINES)
CFLAGS_X64=-Wall -Werror -Wextra -pedantic -ansi -m64 $(DEFINES)

CFLAGS_X86_D=-g -Wall -Werror -Wextra -pedantic -ansi -m32 $(DEFINES_D)
CFLAGS_X64_D=-g -Wall -Werror -Wextra -pedantic -ansi -m64 $(DEFINES_D)

$(OBJ_DIR_X86)/xml.o:
	$(CC) $(CFLAGS_X86) -c -I$(INC_DIR) $(SRC_DIR)/xml.c -o $(OBJ_DIR_X86)/xml.o
$(OBJ_DIR_X64)/xml.o:
	$(CC) $(CFLAGS_X64) -c -I$(INC_DIR) $(SRC_DIR)/xml.c -o $(OBJ_DIR_X64)/xml.o

$(OBJ_DIR_X86)/main.o:
	$(CC) $(CFLAGS_X86) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X86)/main.o
$(OBJ_DIR_X64)/main.o:
	$(CC) $(CFLAGS_X64) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X64)/main.o


$(OBJ_DIR_X86)/xml_d.o:
	$(CC) $(CFLAGS_X86_D) -c -I$(INC_DIR) $(SRC_DIR)/xml.c -o $(OBJ_DIR_X86)/xml_d.o
$(OBJ_DIR_X64)/xml_d.o:
	$(CC) $(CFLAGS_X64_D) -c -I$(INC_DIR) $(SRC_DIR)/xml.c -o $(OBJ_DIR_X64)/xml_d.o

$(OBJ_DIR_X86)/main_d.o:
	$(CC) $(CFLAGS_X86_D) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X86)/main_d.o
$(OBJ_DIR_X64)/main_d.o:
	$(CC) $(CFLAGS_X64_D) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X64)/main_d.o


$(TARGET_PATH_X86): $(OBJ_DIR_X86)/xml.o $(OBJ_DIR_X86)/main.o
	$(AR) $(TARGET_PATH_X86) $(OBJ_DIR_X86)/xml.o
	$(CC) $(CFLAGS_X86) -static $(OBJ_DIR_X86)/main.o -L$(BIN_DIR_X86) -lxml -o $(BIN_DIR_X86)/main.out

$(TARGET_PATH_X64): $(OBJ_DIR_X64)/xml.o $(OBJ_DIR_X64)/main.o
	$(AR) $(TARGET_PATH_X64) $(OBJ_DIR_X64)/xml.o
	$(CC) $(CFLAGS_X64) -static $(OBJ_DIR_X64)/main.o -L$(BIN_DIR_X64) -lxml -o $(BIN_DIR_X64)/main.out


$(TARGET_PATH_X86_D): $(OBJ_DIR_X86)/xml_d.o $(OBJ_DIR_X86)/main_d.o
	$(AR) $(TARGET_PATH_X86_D) $(OBJ_DIR_X86)/xml_d.o
	$(CC) $(CFLAGS_X86_D) -static $(OBJ_DIR_X86)/main_d.o -L$(BIN_DIR_X86) -lxml_d -o $(BIN_DIR_X86)/main_d.out

$(TARGET_PATH_X64_D): $(OBJ_DIR_X64)/xml_d.o $(OBJ_DIR_X64)/main_d.o
	$(AR) $(TARGET_PATH_X64_D) $(OBJ_DIR_X64)/xml_d.o
	$(CC) $(CFLAGS_X64_D) -static $(OBJ_DIR_X64)/main_d.o -L$(BIN_DIR_X64) -lxml_d -o $(BIN_DIR_X64)/main_d.out


all_x86: create_dirs_x86 clean_x86 mrproper_x86 $(TARGET_PATH_X86)
all_x64: create_dirs_x64 clean_x64 mrproper_x64 $(TARGET_PATH_X64)

all_x86_d: create_dirs_x86 clean_x86_d mrproper_x86_d $(TARGET_PATH_X86_D)
all_x64_d: create_dirs_x64 clean_x64_d mrproper_x64_d $(TARGET_PATH_X64_D)

