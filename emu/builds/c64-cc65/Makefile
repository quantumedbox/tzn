NAME := ROM
BASE := ../..
DEF := -DTZN_HTRM
INC := -I $(BASE)/include/ -I $(BASE)/../include/

MAIN := $(BASE)/src/main/tzncli.c
TRMSRC := $(BASE)/src/devices/tzntrm.c $(BASE)/src/devices/trm/tznc64.c
CTRSRC := $(BASE)/src/devices/tznctr.c $(BASE)/src/devices/ctr/tznlc.c
SRC := $(MAIN) $(TRMSRC) $(CTRSRC) $(wildcard $(BASE)/src/*.c)

CCFLAGS := --standard c89 -r --check-stack

RELEASEFLAGS := -Os -Oi -Or -DTZN_RELEASE
DEBUGFLAGS := -g

.PHONY: all clean

all: clean debug

release: CCFLAGS += $(RELEASEFLAGS)
release: $(NAME)

debug: CCFLAGS += $(DEBUGFLAGS)
debug: $(NAME)

$(NAME):
	cl65 -t c64 $(CCFLAGS) $(INC) $(DEF) -o $@ $(SRC)

clean: cleanobj
	$(RM) $(NAME)

cleanobj:
	$(RM) -r $(BASE)/*.o
