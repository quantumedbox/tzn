# TZN Architecture
TZN is computer architecture designed to be emulated on top of variety of 8 bit systems, providing common runtime for small utilities and programs

Addressing is 16 bit, little endian

One potential benefit of using instruction set dispatch is that it might reduce the ROM size as many commonly used code primitives are naturally reused, which is potentially quite valuable in context of systems with strict memory limitations

Ultimate purpose of it is to make single architecture to build tools on top

## Devices
Devices are mapped at first page in memory (address range 0x0000-0x00FF), where each of them takes exactly 16 bytes

### ctr
    Device at index 0x00
- Always guaranteed to be present

| Register | Name            | Access | Description  |
| -------- | --------------- | ------ | ------------ |
| 0x00     | Host ID         | R      | Read-only value holding identity of host in which TZN is running [^list of hosts] |
| 0x01     | Device ID       | R/WF   | Value containing device that is reported in other device registers, valid values are 0x01-0x0F |
| 0x02     | Device Present  | R      | Says whether particular device hinted by Device ID register is present |
| 0x03     | Memory Pages    | R      | Read-only value specifying how many pages of memory are available, including device mapped page, where each page is 256 bytes |

[^list of hosts]: tzn-spec.md#hosts

### trm
    Device at index 0x01
- Every register isn't valid unless trm is initialized (Register 0x1F holds non-zero)
- On initialization cursor visibility and its position is not set to anything in particular
- There's no guarantee on the size of terminal display nor a way to set it, which is deliberate choice for the sake of portability
- By convention character set is ASCII (Whether extended ASCII is supported or not varies, but 0x20-0x7F bytes are guaranteed to be valid)

| Register | Name            | Access | Description  |
| -------- | --------------- | ------ | ------------ |
| 0x10     | Screen Width    | R      | Read-only value containing terminal width in characters |
| 0x11     | Screen Height   | R      | Read-only value containing terminal height in characters |
| 0x12     | Cursor X        | R/WF   | |
| 0x13     | Cursor Y        | R/WF   | |
| 0x14     | Cursor Visibility | R/WF | Non-zero value means that cursor is visible |
| 0x15     | Char Set        | WF     | Set screen cell pointed at Cursor X/Y |
| 0x16     | Char Put        | WF     | Outputs byte to screen buffer where position is pointed by Cursor X/Y registers, after which Cursor X is incremented and its carry is added to Cursor Y |
| 0x1F     | Status          | R/WF   | Holds non-zero value when trm is initialized. Flushing will cause initialization of trm if non-zero value is written, otherwise it will cause deinitialization |

<!-- To consider: -->
<!-- | 0x0F     | Char Mode       | R/W    | Dictate how incoming bytes are interpreted, default value is TRM_CHMOD_ASCII | -->
<!-- | 0x07     | Char Get        | FR     | Get screen cell pointed at Cursor X/Y | -->

<!-- Template: -->
<!-- | Register | Name            | Access | Description  | -->
<!-- | -------- | --------------- | ------ | ------------ | -->
<!-- | 0x0x     |                 |        |              | -->

*Notes*:
- Read-only access doesn't guarantee that memory registers will not be rewritable. User should never attempt to write to them, as it will lead to undefined behavior. For example, many devices could only set their read-only registers on startup, assuming that this data will be persistent in whole lifespan of host

## Hosts
List of all platforms that are somewhat supported at this point in time

| Name  | Identity | Description | trm |
| ----- | -------- | ----------- | --- |
| Self  | 0x00     | Virtual architecture that assumes the same PC on which runtime was compiled on | :heavy_check_mark: (vt100) |
| MSDOS | 0x01     | Intel 80806-based | :heavy_check_mark: (vt100) |
| C64   | 0x02     | Commodore 64, MOS 6502-based | :grey_question: |

Support descriptions:
- :heavy_check_mark: Full
- :grey_question: Partial
- :x: None
