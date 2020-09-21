BIN_DIR := ./bin
INC_DIR := ./include
LIB_DIR := ./lib
OBJ_DIR := ./obj
SRC_DIR := ./src
VPATH := $(SRC_DIR)
CXXFLAGS := -O2 -I $(INC_DIR) #-pedantic-errors -Wall -Wextra -Werror

SRC := $(SRC_DIR)/vocabulary.c $(SRC_DIR)/helper_functions.c $(SRC_DIR)/mainfile.c

OBJECTS := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC:%.c=%.o))
LIBS := $(LIB_DIR)/vocabulary.a $(LIB_DIR)/helper_functions.a
HEADERS := $(INC_DIR)/vocabulary.h $(INC_DIR)/helper_functions.h
EXEC := $(BIN_DIR)/plagChecker

.PHONY: all clean execute
all: $(EXEC)
#	gcc mainfile.o -o plagiarsimChecker -L. -lvocabulary -lhelper_functions -lm

$(BIN_DIR)/plagChecker: $(OBJECTS) $(LIBS)
	@mkdir -p $(BIN_DIR)
	gcc $(CXXFLAGS) -o $(BIN_DIR)/plagChecker $^ -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	gcc $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/%.a: $(OBJ_DIR)/%.o
	@mkdir -p $(LIB_DIR)
	ar cr $@ $<

clean :
	$(RM) $(OBJECTS) $(LIBS) $(BIN_DIR)/plagChecker

execute:
	$(BIN_DIR)/plagChecker input.txt corpus_files