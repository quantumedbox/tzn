# TZN-EMU-DOS
## Installation
- Get *DosBox*: https://www.dosbox.com/
- Get *Borland C++ Compiler*: https://winworldpc.com/product/borland-c/30
- Clone this repo in your DosBox mount directory
- Set *INSTFLD* environment variable to folder in which disk images will be dumped (By entering "set INSTFLD=C:/tmp" for example)
- Copy *INST.BAT* from *dos-bcc* folder into the folder containing Borland Compiler disk images
- Run *INST.BAT* to dump images
- Run *INSTALL.EXE* in folder in which data was unpacked
- Make sure you're installing into *C:\BORLANDC\BIN* (Default)
- Finally you could run *BUILD.BAT* to build TZN-EMU

## Problems
- Currently cpu memory array is reserved at compile time which makes binary size quite huge, maybe there's way to defer its creation similar to .bss
- Feel free to ask me about problems encountered at veclavtalica@protonmail.com
