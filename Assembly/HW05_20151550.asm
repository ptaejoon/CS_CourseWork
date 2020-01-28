TITLE HW5
; 20151550 ������

INCLUDE Irvine32.inc

.data
str1in BYTE "The 1st string<<=24> : ",0
str2in BYTE "The 2nd string<<=24> : ",0
strout BYTE "Merged string : ",0
strend BYTE "bye!",0
str1 BYTE 26 DUP(0)
str2 BYTE 26 DUP(0)
str1size DWORD 0
str2size DWORD 0

.code


main PROC
Restart1:
	mov edx,OFFSET str1in
	call WriteString
	mov edx,OFFSET str1
	mov ecx,SIZEOF str1
	call ReadString
	cmp eax,0
	JE Ending ;�׳ɿ��͸�����
	mov str1size,eax
	mov ecx,eax
	mov esi,OFFSET str1 ; valid check�� �����ϱ� ���� �־��� ��Ǵ�� register���� �������ش�.
	dec esi
	call Valid_Check
	cmp edx,0
	JE Restart1
	jmp validcheck1

Restart2:
	mov edx,OFFSET str2in
	call WriteString
	mov edx,OFFSET str2
	mov ecx,SIZEOF str2

	call ReadString
	cmp eax,0
	JE Restart2 ; �ι�° ��Ʈ���� ���͸� �ĵ� �ٽù޾ƾ���
	mov str2size,eax
	mov ecx,eax
	mov esi,OFFSET str2 ; valid check�� �����ϱ� ���� �־��� ��Ǵ�� register���� �������ش�.
	dec esi
	call Valid_Check
	cmp edx,0
	JE Restart2
	jmp validcheck2

;������� ���� ��Ʈ���� �ް� ������ ��Ʈ������ Ȯ���ϴ� �����̴�.
validcheck1:
	mov esi,OFFSET str1
	dec esi
	mov ecx,str1size ; �־��� ���Ǵ�� ecx,esi���� ��ȯ��Ų���� �Լ��� ȣ��
	call Check_Sorted
	cmp eax,0
	JE Restart1
	jmp Restart2 ; ���� ���ø� ������ ��, 1��° ��Ʈ���� non-decreasing order���� Ȯ�� �� �������� 2��° ��Ʈ���� �����Ƿ� ������ �����־���.

validcheck2:
	mov esi,OFFSET str2
	dec esi
	mov ecx,str2size ; �־��� ���Ǵ�� ecx,esi���� ��ȯ��Ų���� �Լ��� ȣ��
	call Check_Sorted
	cmp eax,0
	JE Restart2

;���������  ��ȿ�� ��Ʈ���� Ȯ���� ���� �� ��Ʈ������ ����� ���ĵ� ��Ʈ������ Ȯ���ϴ� �����̴�.

merging:
	mov edx,OFFSET strout
	call WriteString
	mov esi,OFFSET str1
	mov edi,OFFSET str2
	call Merge ; �� �Լ��� �θ��� ��, ��Ʈ�� �� ���� �ּҰ��� esi, edi�� ���� ����
	jmp Restart1

; Merge �Լ��� �ҷ��� ���յ� string�� ȣ��
; ù ��Ʈ�� �Է��� ������ �ƴϸ� ��� �����ϹǷ� Ending���� ���� ���ϰ� jmp ���


Ending: ; �Լ� �����ϴ� �κ�
	mov edx,OFFSET strend
	call WriteString
	call crlf
exit
main ENDP

Valid_Check PROC
;input : ecx = string size(excluding EOS 0)
;esi = start of the string
;output : edx = 1 if valid, 0 otherwise

	cmp ecx,24
	ja edx0
L1:
	inc esi
	mov al,0
	cmp al,[esi]
	je edx1
	mov al,48
	cmp al,[esi]
	ja edx0
	mov al,57
	cmp al,[esi]
	jae L1
	mov al,65
	cmp al,[esi]
	ja edx0
	mov al,90
	cmp al,[esi]
	jae L1
	mov al,97
	cmp al,[esi]
	ja edx0
	mov al,122
	cmp al,[esi]
	jae L1  ; �� ������ 0���� 9, a���� z A,Z ���̿� �ִ� �������� Ȯ�����ִ� �κ�
edx0:
	mov edx,0;
	ret 
edx1:
	mov edx,1;
	ret

Valid_Check ENDP

Check_Sorted PROC
;input : ecx = string size ( excluding EOS 0 )
;	     esi = start of the string
;output : eax = 1 if non-decreasing, 0 otherwise
	mov dl,0
L2:
	inc esi
	mov al,[esi]
	inc esi
	cmp dl,[esi]
	je eax1
	cmp al,[esi]
	JA eax0
	dec esi
	jmp L2 ; ��Ʈ���� ��� �κ��� üũ�ϸ鼭 ������ ���� �ʴ� �κ��� ������ eax0����, ������ eax1�� ������


eax0:
	mov eax,0
	ret
eax1:
	mov eax,1
	ret
Check_Sorted ENDP

Merge PROC
LOCAL str1val : BYTE,str2val : BYTE
;input :
;esi = start of the string1
;edi = start of the string2
;output : returns nothing, but prompt merged string
goback:
	mov al,[esi]
	mov str1val,al
	mov al,[edi]
	mov str2val,al
	cmp str1val,0
	je str1end
	cmp str2val,0
	je str2end ; �� ��Ʈ���� �Է��� ���� ������ �� ���� �ٸ� ������ ���� �����Ѵ�.
	mov al,str2val
	cmp str1val,al
	ja str1isbig ; str1�� �ִ� ���� �� Ŭ ��� , str2�� �ִ� �ѱ��ڸ� ���
	mov al,str1val ; str1 is big ���� ���� �ʴ� ��� , str1�� �ִ� ���� �� �۴ٴ� ���̹Ƿ� str1�� �ִ� �ѱ��� ���
	call writeChar
	inc esi
	jmp goback 

str1isbig: 
	mov al,str2val
	call writeChar
	inc edi
	jmp goback

str1end:
	cmp str2val,0 ; str1�� ��Ʈ�� �Է��� �� ������ �� str2�κ��� ��Ʈ���� ����
	je bothend
	mov al,str2val
	call writeChar
	inc edi
	jmp goback

str2end: ; str2�� ��Ʈ�� �Է��� �� ������ �� str1�κ��� ��Ʈ���� ����
	cmp str1val,0
	je bothend
	mov al,str1val
	call writeChar
	inc esi
	jmp goback

	
bothend:
	call crlf
	ret

Merge ENDP


ret
END main