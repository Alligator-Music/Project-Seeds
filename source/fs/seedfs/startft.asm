
db 'sdkrnl  bin'
dd 50   ; start sector
dd 32   ; size sectors
db 0x01 ; type of file (sys file)

db 'prog    bin'
dd 0    ; start sector
dd 1    ; size sectors
db 0x01 ; type of file (sys file)

times 16384-($-$$) db 0 ; 16kb amount of data
