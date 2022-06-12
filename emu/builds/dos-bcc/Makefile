CC = BCC
INC = tzn\\emu\\include;tzn\\include;tzn\\emu\\src
SRCDIR = tzn\\emu\\src
FLAGS = -DMSDOS
RELEASEFLAGS = $(FLAGS) -G -b- -A -h -Z -DNDEBUG
DEBUGFLAGS = $(FLAGS) -v -y -lm
IDENT8 = -i8
!if $d(TZN_RLS)
CCFLAGS = -c -I$(INC) $(RELEASEFLAGS)
!else
CCFLAGS = -c -I$(INC) $(DEBUGFLAGS)
!endif
OBJFILES = tzncli.obj

tzn-emu.exe: $(OBJFILES)
	$(CC) -v $(OBJFILES)

tzncli.obj:
	$(CC) $(CCFLAGS) $(IDENT8) $(SRCDIR)\\main\\tzncli.c
