TITLE HW4
; 20151550 ������

INCLUDE Irvine32.inc

.data
strin BYTE "Input String : ",0
strout BYTE "Output String : ",0
strend BYTE "bye!",0
strsum BYTE "Sum : ",0
stri BYTE 51 DUP(0)
strnew BYTE 51 DUP(0)
count DWORD 0
count2 DWORD 0
count3 DWORD 0
count4 DWORD 0
count5 DWORD 0
count6 DWORD 0
count7 DWORD 0
count8 DWORD 0
blank BYTE 0
evennum BYTE 0
n BYTE 0
stringnum BYTE 0
tempstart DWORD 0
tempend DWORD 0
tempscheck BYTE 0
tempecheck BYTE 0
temp2 BYTE 0
esisave DWORD 0

.code


main PROC
Restart:
	mov edx,OFFSET strin
	call WriteString
	mov edx,OFFSET stri
	mov ecx,SIZEOF stri
	call ReadString
	mov tempscheck, 0
	mov tempecheck, 0

	cmp eax,0
	JE Ending
	;�׳ɿ��͸�����
	cmp eax,50
	JA Restart
	;�����������
	call work ; ��ȯ ����

	mov edx,OFFSET strout 
	call WriteString
	mov esi, 0
	sub count,1
	mov eax,count
	mov count5,eax
;��ȯ ���� �͵��� �������� ���
Chull:
	cmp esi,count5
	ja gg
	
	movzx eax,stri[esi]
	mov count4,esi
	sub count,esi
	mov esi,count
	mov strnew[esi],al
	mov eax,count5
	mov count,eax
	mov esi,count4
	inc esi

	jmp Chull
gg:
	mov eax,1
	mov ecx,SIZEOF strnew
	mov edx,OFFSET strnew
	call WriteString
	call crlf
;���� ������� �迭�� �ʱ�ȭ
chogiwha:
	mov esi,0
chogiwha2:
	mov strnew[esi],0
	inc esi
	cmp esi,count5
	je Restart

	jmp chogiwha2

;�����°�
Ending:
	mov edx,OFFSET strend
	call WriteString
	call crlf
exit

	
main ENDP ; ���ι� ����. ���� �� �Ѱų� ����ġ�� ����

;����������Լ�
work PROC
	mov count, eax
	mov blank, 0
	mov esi, -1
	mov evennum, 0
	mov count2, eax
	mov esisave, esi
L1: ; ����Ȯ��
	inc esisave
	mov esi,esisave
	
	cmp esi, count
	jae findn
	movzx eax,stri[esi]
	cmp eax,9
	je sptotab
L2:
	cmp eax,32
	je DONE
	jmp charac
	;������� �� ���� �ܾ��� �� �ܾ���� charac���� ����
	
	jmp L1

comback:
	cmp tempscheck,1
	jne L1
	cmp tempecheck,1
	jne L1
	mov tempscheck,0
	mov tempecheck,0
	mov esi,tempstart
	mov count7,esi
	mov esi,tempend
	mov count8,esi
temploop:
	mov eax,count8
	cmp count7,eax
	jae tempgg
	mov esi,count8
	movzx eax,stri[esi] ; �ڿ� �ְ�����
	mov temp2,al ; �װ� temp2�� �α�
	mov esi,count7 ; �� �հ� �ҷ���
	movzx eax,stri[esi] ; �װ� eax�� ����
	mov esi,count8 ; 
	mov stri[esi],al ; �ҷ��°� �� ���������� ����
	movzx eax,temp2 ; �� �������� �ҷ���
	mov esi,count7 
	mov stri[esi],al ; �װ� �� �տ� ������
	dec count8 
	inc count7 
	jmp temploop
tempgg:
	jmp L1
sptotab:
	mov stri[esi],32
	movzx eax,stri[esi]
	jmp L2

DONE: ;�����̸� ���°�
	inc blank
DONEy: ; ���� ���ڰ� �����̸� ���ƿ��� ��
	inc esi
	movzx eax,stri[esi]
	cmp eax,9
	je sptotab2
L3:
	cmp eax,32
	je DONEx
	dec esi
	jmp L1

sptotab2:
	mov stri[esi],32
	mov eax,32
	jmp L3

DONEx: ; ���� ���ڵ� �����̸� ������ ��
	dec esi

	mov count2,esi
DONEx2: ;��͹�. ����ó���ϴ� ��
	
	mov eax,count ;���� �迭 ���� �Űܼ� 1 �ٿ��� ��
	dec eax
	cmp esi,eax
	je DONEz
	inc esi
	movzx eax, stri[esi]
	dec esi
	mov stri[esi],al ;�Ű���
	inc esi
	jmp DONEx2

DONEz:
	
	mov stri[esi],0
	mov esi,count2 
	dec count
	jmp DONEy

findn:	
	mov esi, 0 
	mov dl,blank
	mov n, dl
	inc n ; �ܾ��� ����
	mov dl,0
	call addrecur
endfunc:
	mov edx,OFFSET strsum
	call WriteString
	call WriteInt
	call crlf
	jmp newRestart
;��������� ����, n ���ϴ� �Լ� ����	
charac:
	test blank,1
	jz holsu
	jmp L1
holsu:
	
	inc esi
	cmp stri[esi],32
	je holsulast
	cmp stri[esi],9
	je holsulast
	cmp stri[esi],0
	je holsulast
	dec esi
	cmp esi,0
	je holsustart
	dec esi
	cmp stri[esi],32
	je holsustart
	inc esi
	jmp L1

holsustart:;
	cmp esi,0
	je imsi
	inc esi
imsi:
	mov tempstart,esi
	mov tempscheck,1
	jmp comback

holsulast:
	dec esi
	dec esi
	cmp stri[esi],32
	je holsustart
	inc esi
	push ecx
	mov ecx,esi
	mov tempend,ecx
	pop ecx
	mov tempecheck,1
	jmp comback

newRestart:
work ENDP ;�Լ�����

;n���ϴ� ����Լ�
addrecur PROC
	
	cmp n,1
	JE same
	add dl, n
	dec n
	call addrecur
	jmp aa
same:
	add dl, 1
	mov al,dl
aa:
	ret

addrecur ENDP
END main