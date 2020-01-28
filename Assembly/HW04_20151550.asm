TITLE HW4
; 20151550 박태준

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
	;그냥엔터면종료
	cmp eax,50
	JA Restart
	;넘으면재시작
	call work ; 변환 시작

	mov edx,OFFSET strout 
	call WriteString
	mov esi, 0
	sub count,1
	mov eax,count
	mov count5,eax
;변환 다한 것들을 역순으로 출력
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
;새로 만들었던 배열을 초기화
chogiwha:
	mov esi,0
chogiwha2:
	mov strnew[esi],0
	inc esi
	cmp esi,count5
	je Restart

	jmp chogiwha2

;끝내는곳
Ending:
	mov edx,OFFSET strend
	call WriteString
	call crlf
exit

	
main ENDP ; 메인문 종료. 글자 수 넘거나 엔터치면 끝남

;사용자정의함수
work PROC
	mov count, eax
	mov blank, 0
	mov esi, -1
	mov evennum, 0
	mov count2, eax
	mov esisave, esi
L1: ; 공백확인
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
	;여기까지 온 것은 단어라는 뜻 단어들은 charac으로 간다
	
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
	movzx eax,stri[esi] ; 뒤에 최고값저장
	mov temp2,al ; 그걸 temp2에 두기
	mov esi,count7 ; 맨 앞값 불러옴
	movzx eax,stri[esi] ; 그걸 eax에 저장
	mov esi,count8 ; 
	mov stri[esi],al ; 불러온걸 맨 마지막값에 저장
	movzx eax,temp2 ; 맨 마지막값 불러옴
	mov esi,count7 
	mov stri[esi],al ; 그걸 맨 앞에 가져옴
	dec count8 
	inc count7 
	jmp temploop
tempgg:
	jmp L1
sptotab:
	mov stri[esi],32
	movzx eax,stri[esi]
	jmp L2

DONE: ;공백이면 가는곳
	inc blank
DONEy: ; 다음 글자가 공백이면 돌아오는 곳
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

DONEx: ; 다음 글자도 공백이면 들어오는 곳
	dec esi

	mov count2,esi
DONEx2: ;재귀문. 공백처리하는 곳
	
	mov eax,count ;원래 배열 수를 옮겨서 1 줄여서 비교
	dec eax
	cmp esi,eax
	je DONEz
	inc esi
	movzx eax, stri[esi]
	dec esi
	mov stri[esi],al ;옮겼음
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
	inc n ; 단어의 개수
	mov dl,0
	call addrecur
endfunc:
	mov edx,OFFSET strsum
	call WriteString
	call WriteInt
	call crlf
	jmp newRestart
;여기까지가 공백, n 구하는 함수 관련	
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
work ENDP ;함수종료

;n구하는 재귀함수
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