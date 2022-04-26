NAME := tznemu
BASE := ../..
INC := -I $(BASE)/include/ -I $(BASE)/../include/ -I $(BASE)/src/

MAIN := $(BASE)/src/main/tzncli.c
SRC := $(MAIN) $(BASE)/src/tio.c

CCFLAGS := --standard c89 -Cl -r

RELEASEFLAGS := -Osir -DTZN_RLS --asm-args --relax-checks --asm-args -s
DEBUGFLAGS := -g --check-stack

# ifndef TZN_RLS
# 	CCFLAGS += $(DEBUGFLAGS)
# else
CCFLAGS += $(RELEASEFLAGS)
# endif

.PHONY: all clean cleanobj

all: clean $(NAME)

asm:
	cl65 -t c64 -S $(CCFLAGS) $(INC) $(SRC)

$(NAME):
	cl65 -t c64 $(CCFLAGS) $(INC) -o $@ $(SRC)

clean: cleanobj
	$(RM) $(NAME)

cleanobj:
	$(RM) -r $(BASE)/*.o
