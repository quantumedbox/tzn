NAME := tznemu
BASE := ../..
INC := -I $(BASE)/include/ -I $(BASE)/../include/ -I $(BASE)/src/
CPU_EXEC := $(BASE)/src/cpus/t6502.s

MAIN := $(BASE)/src/main/tzncli.c

ASMFLAGS := --relax-checks --smart
CCFLAGS := --standard c89 -Cl -r $(addprefix --asm-args ,$(ASMFLAGS)) -DT_CPUASM

RELEASEFLAGS := -Osir -DTZN_RLS
DEBUGFLAGS := -g --check-stack

# ifndef TZN_RLS
# 	CCFLAGS += $(DEBUGFLAGS)
# else
CCFLAGS += $(RELEASEFLAGS)
# endif

.PHONY: all clean cleanobj

all: clean $(NAME)

asm: $(CPU_EXEC:.s=.o)
	cl65 -t c64 -S $(CCFLAGS) $(INC) $(MAIN) $(notdir $<)

$(NAME): $(CPU_EXEC:.s=.o)
	cl65 -t c64 $(CCFLAGS) $(INC) -o $@ $(MAIN) $(notdir $<)

%.o: %.s
	ca65 -t c64 $(ASMFLAGS) -o $(notdir $@) $<

clean: cleanobj
	$(RM) $(NAME)
	$(RM) $(notdir $(CPU_EXEC:.c=.o))

cleanobj:
	$(RM) -r $(BASE)/*.o
