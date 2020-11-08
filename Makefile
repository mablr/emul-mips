# Inspiré de l'exemple https://stackoverflow.com/a/2484343
PROG = emul-mips

CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
LDFLAGS = -lm
MODULES = main  
SRC_D = $(addprefix src/,$(MODULES))
OBJ_D = $(addprefix obj/,$(MODULES))
SRC = $(foreach sd,$(SRC_D),$(wildcard $(sd)/*.c))
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

vpath %.c $(SRC_D)

all: createobjdirs $(PROG) clean

createobjdirs: $(OBJ_D)
$(OBJ_D):
	@mkdir -p $@

$(PROG): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

define create_obj
$1/%.o: %.c
	$(CC) -o $$@ -c $$< $(CFLAGS)
endef

$(foreach obj,$(OBJ_D),$(eval $(call create_obj,$(obj))))

clean:
	@rm -rf $(OBJ_D)