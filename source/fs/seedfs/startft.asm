
db 'sdkrnl  bin'                                     ; 11 bytes
dd 50    ; start sector                              ; 4 bytes
dd 64    ; size sectors                              ; 4 bytes
db 0x01  ; type of file (sys file)                   ; 1 byte
dd 16384 ; size bytes                                ; 4 bytes
dd 0     ; offset from filetable to next file in dir ; 4 bytes (0 because only file)
dd 0     ; reserved                                  ; 4 bytes

times 16384-($-$$) db 0 ; 16kb amount of data
