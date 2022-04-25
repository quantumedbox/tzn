NAME := ROM
BASE := ../..
INC := -I $(BASE)/include/ -I $(BASE)/../include/ -I $(BASE)/src/devices/

MAIN := $(BASE)/src/main/tzncli.c
SRC := $(MAIN) $(wildcard $(BASE)/src/*.c)

CCFLAGS := --standard c89 -Cl -r

RELEASEFLAGS := -Os -Oi -Or -DTZN_RLS --asm-args --relax-checks --asm-args -s
DEBUGFLAGS := -g --check-stack

ifdef TZN_RLS
	CCFLAGS += $(RELEASEFLAGS)
else
	CCFLAGS += $(DEBUGFLAGS)
endif

.PHONY: all clean cleanobj

all: clean $(NAME)

$(NAME):
	cl65 -t c64 $(CCFLAGS) $(INC) -o $@ $(SRC)

clean: cleanobj
	$(RM) $(NAME)

cleanobj:
	$(RM) -r $(BASE)/*.o
