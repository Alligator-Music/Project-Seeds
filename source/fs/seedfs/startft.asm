db 'load    bin'
dd 1    ; start sector
dd 16   ; size sectors
db 0x01 ; type of file (sys file)

db 'sdkrnl  bin'
dd 50   ; start sector
dd 16   ; size sectors
db 0x01 ; type of file (sys file)

times 16384-($-$$) db 0 ; 16kb amount of data
