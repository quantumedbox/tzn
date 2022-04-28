# TZN
TZN is computer architecture designed to be emulated on top of variety of 8 bit systems, providing common runtime for small utilities and programs

Addressing is 16 bit, little endian

One potential benefit of using instruction set dispatch is that it might reduce the ROM size as many commonly used code primitives are naturally reused, which is potentially quite valuable in context of systems with strict memory limitations

# Devices
Devices are mapped at first page in memory (address range 0x0000-0x00FF), where each of them takes exactly 16 bytes

## ctr
    Device at index 0x00
- Always guaranteed to be present

| Register | Name            | Access | Description  |
| -------- | --------------- | ------ | ------------ |
| 0x00     | Device ID       | R/W    | Used on flushing, valid values are 0x01-0x0F |
| 0x01     | Device Present  | RF     | Says whether particular device hinted by Device ID register is present |
| 0x02     | Memory Pages    | R      | Read-only value specifying how many pages of memory are available, where each page is 256 bytes |
| 0x0F     | Host ID         | R      | Read-only value holding identity of host in which TZN is running [^list of hosts] |

[^list of hosts]: tzn-spec.md#hosts

## trm
    Device at index 0x01
- On startup cursor is visible and its position is at (0, 0)
- There's no guarantee on the size of terminal display nor a way to set it, which is deliberate choice for the sake of portability
- By convention character set is ASCII (Whether extended ASCII is supported or not varies, but 0x20-0x7F bytes are guaranteed to be valid)

| Register | Name            | Access | Description  |
| -------- | --------------- | ------ | ------------ |
| 0x00     | Screen Width    | R      | Read-only value containing terminal width in characters |
| 0x01     | Screen Height   | R      | Read-only value containing terminal height in characters |
| 0x02     | Cursor X        | R/W    | Used on flushing |
| 0x03     | Cursor Y        | R/W    | Used on flushing |
| 0x04     | Cursor Visibility | R/WF | Non-zero value means that cursor is visible |
| 0x05     | Char Access     | RF/WF  | Access to bytes in screen buffer pointed at Cursor X/Y, outputting doesn't change Cursor X/Y |
| 0x06     | Char Out        | WF     | Outputs byte to screen buffer where position is pointed by Cursor X/Y registers, after which Cursor X is incremented and its carry is added to Cursor Y |
<!-- | 0x0x     |                 |        | | -->
<!-- | 0x0F     | Char Mode       | R/W    | Dictate how incoming bytes are interpreted, default value is TRM_CHMOD_ASCII | -->

# Hosts
List of all platforms that are somewhat supported at this point in time

| Name  | Identity | Description | trm |
| ----- | -------- | ----------- | --- |
| Self  | 0x00     | Virtual architecture that assumes the same PC on which runtime was compiled on | :heavy_check_mark: |
| MSDOS | 0x01     | Intel 80806-based | :heavy_check_mark: |
| C64   | 0x02     | Commodore 64, MOS 6502-based | :interrobang: |

- :heavy_check_mark: Full support
- :interrobang: Partial support
- :x: No support
