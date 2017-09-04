// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(LOOP)
    @i
    M=1
    @KBD
    D=M
    @WHITE
    D;JLE

//BLACK
    @color
    M=-1
    @FILL
    0;JMP
(WHITE)
    @color
    M=0
    @FILL
    0;JMP

(FILL)
    @i
    D=M
    @8192
    D=D-A
    @LOOP
    D;JGT
    @i
    D=M
    @SCREEN
    D=D+A
    @tmpaddress
    M=D-1
    @color
    D=M
    @tmpaddress
    A=M
    M=D
    @i
    M=M+1
    @FILL
    0;JMP

