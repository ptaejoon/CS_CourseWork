TITLE HW3
; 20151550 นฺลยมุ

INCLUDE Irvine32.inc

.data
aName DWORD "Assembly Language for x86      Processors"
nameSize = ($ - aName) - 1
.code
main PROC
	mov ecx, nameSize
	mov esi,0
L1: movzx eax,aName[esi]
	push eax
	inc esi
	Loop L1

	mov ecs,nameSize
	mov esi,0
L2: pop eax
	mov aName[esi],al
	inc esi
	Loop L2

	
	call DumpRegs	; Display registers.
	exit
main ENDP
END main