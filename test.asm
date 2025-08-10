; nasm --version				> Will be using NASM for this project
; nasm -felf64 test.asm			> Compile the 64-bit version
; ld test.o -o test				> Run the linker and create the executable
; ./test						> Run the executable
; echo $?						> Display the last error encountered

; This is the assembly code required for the C statement "exit(21);"
global _start

_start:
	mov rax, 60					; Exit = syscall 60
	mov rdi, 21					; Move error code 21 into rdi
	syscall