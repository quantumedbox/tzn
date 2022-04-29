.fopt compiler,"cc65 v 2.19 - Git c34d6f0"
.setcpu "6502"
.smart on
.autoimport on
.case on
.importzp ptr1, sreg
.macpack  longbranch

.import   _tCpuInit
.import   _tCpuRgPC
.import   _tCpuRgA
.import   _tCpuRgB
.import   _tCpuRgC
.import   _tCpuRgD
.import   _tCpuRgSL
.import   _tCpuRgSH
.import   _tCpuRgF
.import   _tCpuRam
.import   _tznDvcRd
.import   _tznDvcWr

.export   _tCpuExec

; TODO Use native carry / underflow / overflow flags for instructions that need it

; ---------------------------------------------------------------
; void __near__ tCpuExec (void)
; ---------------------------------------------------------------

.segment  "CODE"

.proc _tCpuExec: near

.segment  "DATA"
; https://www.nesdev.org/wiki/Jump_table
table_low:
  .byte <L0090
  .byte <L0091
  .byte <L0092
  .byte <L0093
  .byte <L0094
  .byte <L0095
  .byte <L0097
  .byte <L0098
  .byte <L0099
  .byte <L009A
  .byte <L009B
  .byte <L009C
  .byte <L009E
  .byte <L009F
  .byte <L00A0
  .byte <L00A1
  .byte <L00A5
  .byte <L00A6
  .byte <L00A7
  .byte <L00A9
  .byte <L00AB
  .byte <L00B0
  .byte <L00A8
  .byte <L00AA
  .byte <L00AD
  .byte <L00B2
  .byte <L00B3
  .byte <L00B4
  .byte <L002B
  .byte <L0030
  .byte <L00B7
  .byte <L00B9
  .byte <L0037
  .byte <L0038
  .byte <L00BB
  .byte <L00C0
  .byte <L00C2
  .byte <L00C4
  .byte <L00C5
  .byte <L00C8
  .byte <L00CB
  .byte <L00CC
  .byte <L004F
  .byte <L0051
  .byte <L0053
  .byte <L0055
  .byte <L0057
  .byte <L0059
  .byte <L005B
  .byte <L00CE
  .byte <L005F
  .byte <L00D4
  .byte <L00D5
  .byte <L0068
  .byte <L006A
  .byte <L006C
  .byte <L0071
  .byte <L0076
  .byte <L0078
  .byte <L007F
  .byte <L0082
  .byte <L0085
  .byte <L00E1
table_high:
  .byte >L0090
  .byte >L0091
  .byte >L0092
  .byte >L0093
  .byte >L0094
  .byte >L0095
  .byte >L0097
  .byte >L0098
  .byte >L0099
  .byte >L009A
  .byte >L009B
  .byte >L009C
  .byte >L009E
  .byte >L009F
  .byte >L00A0
  .byte >L00A1
  .byte >L00A5
  .byte >L00A6
  .byte >L00A7
  .byte >L00A9
  .byte >L00AB
  .byte >L00B0
  .byte >L00A8
  .byte >L00AA
  .byte >L00AD
  .byte >L00B2
  .byte >L00B3
  .byte >L00B4
  .byte >L002B
  .byte >L0030
  .byte >L00B7
  .byte >L00B9
  .byte >L0037
  .byte >L0038
  .byte >L00BB
  .byte >L00C0
  .byte >L00C2
  .byte >L00C4
  .byte >L00C5
  .byte >L00C8
  .byte >L00CB
  .byte >L00CC
  .byte >L004F
  .byte >L0051
  .byte >L0053
  .byte >L0055
  .byte >L0057
  .byte >L0059
  .byte >L005B
  .byte >L00CE
  .byte >L005F
  .byte >L00D4
  .byte >L00D5
  .byte >L0068
  .byte >L006A
  .byte >L006C
  .byte >L0071
  .byte >L0076
  .byte >L0078
  .byte >L007F
  .byte >L0082
  .byte >L0085
  .byte >L00E1

.segment  "CODE"
  jsr     _tCpuInit
L0002:
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  pha
  inc     _tCpuRgPC
  bne     L0005
  inc     _tCpuRgPC+1
L0005:
  ; TODO Check whether instruction is in range? Could hinder the performance quite a bit tho
  pla
  tax
  lda     table_low,x
  sta     ptr1
  lda     table_high,x
  sta     ptr1+1
  jmp     (ptr1) ; Undefined op will lead to jump in unspecified position
L0090:
  sta     _tCpuRgA
  jmp     L0002
L0091:  lda     _tCpuRgA
  sta     _tCpuRgB
  jmp     L0002
L0092:  lda     _tCpuRgA
  sta     _tCpuRgC
  jmp     L0002
L0093:  lda     _tCpuRgA
  sta     _tCpuRgD
  jmp     L0002
L0094:  lda     _tCpuRgA
  sta     _tCpuRgSL
  jmp     L0002
L0095:  lda     _tCpuRgA
  sta     _tCpuRgSH
  jmp     L0002
L0097:  lda     _tCpuRgB
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgC
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgA
  ldy     #$00
  sta     (ptr1),y
  jmp     L0002
L0098:  lda     _tCpuRgB
  sta     _tCpuRgA
  jmp     L0002
L0099:  lda     _tCpuRgC
  sta     _tCpuRgA
  jmp     L0002
L009A:  lda     _tCpuRgD
  sta     _tCpuRgA
  jmp     L0002
L009B:  lda     _tCpuRgSL
  sta     _tCpuRgA
  jmp     L0002
L009C:  lda     _tCpuRgSH
  sta     _tCpuRgA
  jmp     L0002
L009E:  lda     _tCpuRgB
  sta     ptr1
  lda     _tCpuRgC
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgA
  jmp     L0002
L009F:  lda     _tCpuRgB
  clc
  adc     _tCpuRgA
  sta     _tCpuRgA
  lda     _tCpuRgA
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00A0:  lda     _tCpuRgA
  sec
  sbc     _tCpuRgB
  sta     M0001
  cmp     _tCpuRgA
  jsr     boolugt
  sta     _tCpuRgF
  lda     M0001
  sta     _tCpuRgA
  jmp     L0002
L00A1:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgB
  jsr     tosumula0
  sta     M0002
  lda     _tCpuRgA
  beq     L00A4
  lda     M0002
  jsr     pusha0
  lda     _tCpuRgA
  jsr     tosudiva0
  cpx     #$00
  bne     L0019
  cmp     _tCpuRgB
  bne     L0019
  txa
  jmp     L00A4
L0019:  lda     #$01
L00A4:  sta     _tCpuRgF
  lda     M0002
  sta     _tCpuRgA
  jmp     L0002
L00A5:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgB
  jsr     tosudiva0
  sta     _tCpuRgA
  jmp     L0002
L00A6:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgB
  jsr     tosumoda0
  sta     _tCpuRgA
  jmp     L0002
L00A7:  inc     _tCpuRgA
  lda     _tCpuRgA
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00A8:  dec     _tCpuRgA
  lda     _tCpuRgA
  cmp     #$FF
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00A9:  inc     _tCpuRgB
  lda     _tCpuRgB
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00AA:  dec     _tCpuRgB
  lda     _tCpuRgB
  cmp     #$FF
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00AB:  inc     _tCpuRgB
  bne     L00AC
  inc     _tCpuRgC
  lda     _tCpuRgC
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00AC:  lda     #$00
  sta     _tCpuRgF
  jmp     L0002
L00AD:  dec     _tCpuRgB
  lda     _tCpuRgB
  cmp     #$FF
  bne     L00AE
  dec     _tCpuRgC
  lda     _tCpuRgC
  cmp     #$FF
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00AE:  lda     #$00
  sta     _tCpuRgF
  jmp     L0002
L00B0:  lda     _tCpuRgB
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgC
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #$00
  lda     #$01
  clc
  adc     (ptr1),y
  sta     (ptr1),y
  jmp     L0002
L00B2:  lda     _tCpuRgB
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgC
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #$00
  lda     (ptr1),y
  sec
  sbc     #$01
  sta     (ptr1),y
  jmp     L0002
L00B3:  lda     _tCpuRgA
  cmp     _tCpuRgB
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00B4:  lda     _tCpuRgA
  cmp     _tCpuRgB
  jsr     boolult
  sta     _tCpuRgF
  jmp     L0002
L002B:  ldy     _tCpuRgA
  lda     _tCpuRam,y
  sta     M0003
  and     #$80
  beq     L00B5
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  jsr     pushax
  lda     M0003
  eor     #$FF
  jsr     tossuba0
  sec
  sbc     #$01
  bcs     L002E
  dex
L002E:  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002
L00B5:  lda     M0003
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  lda     #$00
  adc     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L0030:  ldy     _tCpuRgA
  lda     _tCpuRam,y
  jsr     pusha0
  lda     _tCpuRgF
  jsr     tosumula0
  sta     M0004
  and     #$80
  beq     L00B6
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  jsr     pushax
  lda     M0004
  eor     #$FF
  jsr     tossuba0
  sec
  sbc     #$01
  bcs     L0033
  dex
L0033:  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002
L00B6:  lda     M0004
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  lda     #$00
  adc     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L00B7:  lda     _tCpuRgA
  sta     _tCpuTemp
  jsr     _tznDvcWr
  jmp     L0002
L00B9:  lda     _tCpuRgB
  sta     ptr1
  lda     _tCpuRgC
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuTemp
  jsr     _tznDvcWr
  jmp     L0002
L0037:  jsr     _tznDvcRd
  lda     _tCpuTemp
  sta     _tCpuRgA
  jmp     L0002
L0038:  jsr     _tznDvcRd
  lda     _tCpuRgB
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgC
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuTemp
  ldy     #$00
  sta     (ptr1),y
  jmp     L0002
L00BB:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC
  ldy     #$00
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L00BD
  dec     _tCpuRgSH
L00BD:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC+1
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L00BF
  dec     _tCpuRgSH
L00BF:  lda     _tCpuRgB
  sta     _tCpuRgPC
  lda     _tCpuRgC
  sta     _tCpuRgPC+1
  jmp     L0002
L00C0:  inc     _tCpuRgSL
  bne     L003D
  inc     _tCpuRgSH
L003D:  lda     _tCpuRgSL
  sta     ptr1
  lda     _tCpuRgSH
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgPC+1
  lda     _tCpuRgPC
  sta     _tCpuRgPC
  inc     _tCpuRgSL
  bne     L0041
  inc     _tCpuRgSH
L0041:  lda     _tCpuRgSL
  sta     ptr1
  lda     _tCpuRgSH
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgPC
  lda     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L00C2:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgA
  ldy     #$00
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  jne     L0002
  dec     _tCpuRgSH
  jmp     L0002
L00C4:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgB
  ldy     #$00
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  jne     L0002
  dec     _tCpuRgSH
  jmp     L0002
L00C5:  inc     _tCpuRgSL
  bne     L00C7
  inc     _tCpuRgSH
L00C7:  lda     _tCpuRgSL
  sta     ptr1
  lda     _tCpuRgSH
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgA
  jmp     L0002
L00C8:  inc     _tCpuRgSL
  bne     L00CA
  inc     _tCpuRgSH
L00CA:  lda     _tCpuRgSL
  sta     ptr1
  lda     _tCpuRgSH
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgB
  jmp     L0002
L00CB:  lda     _tCpuRgF
  eor     #$01
  sta     _tCpuRgF
  jmp     L0002
L00CC:  lda     _tCpuRgF
  and     #$FE
  sta     ptr1
  lda     _tCpuRgF
  lsr     a
  and     #$01
  ora     ptr1
  sta     _tCpuRgF
  jmp     L0002
L004F:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgA
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0051:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgB
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0053:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgC
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0055:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgD
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0057:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgSL
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0059:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgSH
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L005B:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  clc
  adc     _tCpuRgA
  sta     _tCpuRgA
  inc     _tCpuRgPC
  bne     L00CD
  inc     _tCpuRgPC+1
L00CD:  lda     _tCpuRgA
  jsr     booleq
  sta     _tCpuRgF
  jmp     L0002
L00CE:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  jsr     tossuba0
  sta     M0005
  inc     _tCpuRgPC
  bne     L00CF
  inc     _tCpuRgPC+1
L00CF:  lda     M0005
  cmp     _tCpuRgA
  jsr     boolugt
  sta     _tCpuRgF
  lda     M0005
  sta     _tCpuRgA
  jmp     L0002
L005F:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     M0006
  inc     _tCpuRgPC
  bne     L00D0
  inc     _tCpuRgPC+1
L00D0:  lda     _tCpuRgA
  jsr     pusha0
  lda     M0006
  jsr     tosumula0
  sta     M0007
  lda     _tCpuRgA
  beq     L00D3
  lda     M0007
  jsr     pusha0
  lda     _tCpuRgA
  jsr     tosudiva0
  cpx     #$00
  bne     L0062
  cmp     M0006
  bne     L0062
  txa
  jmp     L00D3
L0062:  lda     #$01
L00D3:  sta     _tCpuRgF
  lda     M0007
  sta     _tCpuRgA
  jmp     L0002
L00D4:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  jsr     tosudiva0
  sta     _tCpuRgA
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L00D5:  lda     _tCpuRgA
  jsr     pusha0
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  jsr     tosumoda0
  sta     _tCpuRgA
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0068:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  cmp     _tCpuRgA
  jsr     booleq
  sta     _tCpuRgF
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L006A:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  cmp     _tCpuRgA
  jsr     boolugt
  sta     _tCpuRgF
  inc     _tCpuRgPC
  jne     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L006C:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     M0008
  inc     _tCpuRgPC
  bne     L00D6
  inc     _tCpuRgPC+1
L00D6:  lda     M0008
  and     #$80
  beq     L00D7
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  jsr     pushax
  lda     M0008
  eor     #$FF
  jsr     tossuba0
  sec
  sbc     #$01
  bcs     L006F
  dex
L006F:  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002
L00D7:  lda     M0008
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  lda     #$00
  adc     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L0071:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  jsr     pusha0
  lda     _tCpuRgF
  jsr     tosumula0
  sta     M0009
  inc     _tCpuRgPC
  bne     L00D8
  inc     _tCpuRgPC+1
L00D8:  lda     M0009
  and     #$80
  beq     L00D9
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  jsr     pushax
  lda     M0009
  eor     #$FF
  jsr     tossuba0
  sec
  sbc     #$01
  bcs     L0074
  dex
L0074:  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002
L00D9:  lda     M0009
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  lda     #$00
  adc     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L0076:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuTemp
  inc     _tCpuRgPC
  bne     L0077
  inc     _tCpuRgPC+1
L0077:  jsr     _tznDvcWr
  jmp     L0002
L0078:  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     M000A
  inc     _tCpuRgPC
  bne     L00DB
  inc     _tCpuRgPC+1
L00DB:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC
  ldy     #$00
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L00DD
  dec     _tCpuRgSH
L00DD:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC+1
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L00DE
  dec     _tCpuRgSH
L00DE:  lda     M000A
  and     #$80
  beq     L00DF
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  jsr     pushax
  lda     M000A
  eor     #$FF
  jsr     tossuba0
  sec
  sbc     #$01
  bcs     L007D
  dex
L007D:  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002
L00DF:  lda     M000A
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  tya
  adc     _tCpuRgPC+1
  sta     _tCpuRgPC+1
  jmp     L0002
L007F:  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  clc
  adc     #$01
  bcc     L0080
  inx
L0080:  sta     ptr1
  txa
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     sreg+1
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     ptr1
  lda     sreg+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     _tCpuRgA
  lda     #$02
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  jcc     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0082:  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  clc
  adc     #$01
  bcc     L0083
  inx
L0083:  sta     ptr1
  txa
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     sreg+1
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     sreg+1
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgA
  ldy     #$00
  sta     (ptr1),y
  lda     #$02
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  jcc     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L0085:  jsr     _tznDvcRd
  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  clc
  adc     #$01
  bcc     L0086
  inx
L0086:  sta     ptr1
  txa
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     sreg+1
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     sreg+1
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuTemp
  ldy     #$00
  sta     (ptr1),y
  lda     #$02
  clc
  adc     _tCpuRgPC
  sta     _tCpuRgPC
  jcc     L0002
  inc     _tCpuRgPC+1
  jmp     L0002
L00E1:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC
  ldy     #$00
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L00E3
  dec     _tCpuRgSH
L00E3:  lda     _tCpuRgSL
  clc
  adc     #<(_tCpuRam)
  sta     ptr1
  lda     _tCpuRgSH
  adc     #>(_tCpuRam)
  sta     ptr1+1
  lda     _tCpuRgPC+1
  sta     (ptr1),y
  dec     _tCpuRgSL
  lda     _tCpuRgSL
  cmp     #$FF
  bne     L008A
  dec     _tCpuRgSH
L008A:  lda     _tCpuRgPC
  ldx     _tCpuRgPC+1
  clc
  adc     #$01
  bcc     L008B
  inx
L008B:  sta     ptr1
  txa
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     sreg+1
  lda     _tCpuRgPC
  sta     ptr1
  lda     _tCpuRgPC+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  lda     (ptr1),y
  sta     ptr1
  lda     sreg+1
  clc
  adc     #>(_tCpuRam)
  sta     ptr1+1
  ldy     #<(_tCpuRam)
  ldx     #$00
  lda     (ptr1),y
  sta     _tCpuRgPC
  stx     _tCpuRgPC+1
  jmp     L0002

.segment  "BSS"

M0001:
  .res  1,$00
M0002:
  .res  1,$00
M0003:
  .res  1,$00
M0004:
  .res  1,$00
M0005:
  .res  1,$00
M0006:
  .res  1,$00
M0007:
  .res  1,$00
M0008:
  .res  1,$00
M0009:
  .res  1,$00
M000A:
  .res  1,$00

.endproc
