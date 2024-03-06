# Project-Seeds
A Custom 32-bit operating system

Features:
 - An MBR and bootloader
 - 32 bit system
 - CLI
 - Custom libc

seeds is a kernel with a simple cli and custom file system

loading structure (what happens when u start the computer):
    MBR - this is loaded by the bios bla bla bla and then loads the seeds loader.
        for more information on this, go look at the source code!!!
    
    seeds loader or load.bin - loads the operating system and enters protected mode.
    
    general kernel stuff:
        so after load.bin loads the kernel, the kernel runs a CLI.
 