;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro to initialize the NES by disabling rendering and clearing RAM values
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.macro INIT_NES
    sei                       ; Disable all IRQ interrupts
    cld                       ; Clear decimal mode (unsupported by the NES)
    ldx #$FF
    txs                       ; Initialize the stack pointer at address $FF
    inx                       ; Increment X, causing a roll-off from $FF to $0
    stx PPU_CTRL              ; Disable NMI
    stx PPU_MASK              ; Disable rendering
    stx $4010                 ; Disable DMC IRQs
    lda #$40
    sta $4017                 ; Disable APU frame IRQ
Wait1stVBlank:                ; Wait for the first VBlank from the PPU
    bit PPU_STATUS            ; Perform a bit-wise check with the PPU_STATUS port
    bpl Wait1stVBlank         ; Loop until bit-7 (sign bit) is 1 (inside VBlank)
    txa                       ; A = 0
ClearRAM:
    sta $0000,x               ; Zero Ram addresses from $0000 to $00FF
    sta $0100,x               ; Zero Ram addresses from $0100 to $01FF
    lda #$FF                  ; Initialize OAM data with $FF to hide sprites
    sta $0200,x               ; Store values in RAM address from $0200 to $02FF
    lda #0                    ; A = 0
    sta $0300,x               ; Zero Ram addresses from $0300 to $03FF
    sta $0400,x               ; Zero Ram addresses from $0400 to $04FF
    sta $0500,x               ; Zero Ram addresses from $0500 to $05FF
    sta $0600,x               ; Zero Ram addresses from $0600 to $06FF
    sta $0700,x               ; Zero Ram addresses from $0700 to $07FF
    inx                       ; X++
    bne ClearRAM              ; Loop until X reaches zero (after roll-off)
Wait2ndVBlank:                ; Wait for the first VBlank from the PPU
    bit PPU_STATUS            ; Perform a bit-wise check with the PPU_STATUS port
    bpl Wait2ndVBlank         ; Loop until bit-7 (sign bit) is 1 (inside VBlank)
.endmacro