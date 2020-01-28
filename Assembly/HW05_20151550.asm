TITLE HW5
; 20151550 박태준

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
	JE Ending ;그냥엔터면종료
	mov str1size,eax
	mov ecx,eax
	mov esi,OFFSET str1 ; valid check를 시행하기 전에 주어진 요건대로 register값을 설정해준다.
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
	JE Restart2 ; 두번째 스트링은 엔터를 쳐도 다시받아야함
	mov str2size,eax
	mov ecx,eax
	mov esi,OFFSET str2 ; valid check를 시행하기 전에 주어진 요건대로 register값을 설정해준다.
	dec esi
	call Valid_Check
	cmp edx,0
	JE Restart2
	jmp validcheck2

;여기까지 각각 스트링을 받고 유요한 스트링인지 확인하는 과정이다.
validcheck1:
	mov esi,OFFSET str1
	dec esi
	mov ecx,str1size ; 주어진 조건대로 ecx,esi값을 변환시킨다음 함수를 호출
	call Check_Sorted
	cmp eax,0
	JE Restart1
	jmp Restart2 ; 실행 예시를 보았을 때, 1번째 스트링이 non-decreasing order인지 확인 한 다음에야 2번째 스트링을 받으므로 순서를 맞춰주었다.

validcheck2:
	mov esi,OFFSET str2
	dec esi
	mov ecx,str2size ; 주어진 조건대로 ecx,esi값을 변환시킨다음 함수를 호출
	call Check_Sorted
	cmp eax,0
	JE Restart2

;여기까지가  유효한 스트링을 확인한 다음 이 스트링들이 제대로 정렬된 스트링인지 확인하는 과정이다.

merging:
	mov edx,OFFSET strout
	call WriteString
	mov esi,OFFSET str1
	mov edi,OFFSET str2
	call Merge ; 이 함수를 부르기 전, 스트링 두 개의 주소값을 esi, edi에 각각 저장
	jmp Restart1

; Merge 함수를 불러서 결합된 string을 호출
; 첫 스트링 입력이 공백이 아니면 계속 실행하므로 Ending으로 가지 못하게 jmp 사용


Ending: ; 함수 종료하는 부분
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
	jae L1  ; 이 과정이 0부터 9, a부터 z A,Z 사이에 있는 글자인지 확인해주는 부분
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
	jmp L2 ; 스트링의 모든 부분을 체크하면서 순서가 맞지 않는 부분이 있으면 eax0으로, 없으면 eax1로 보낸다


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
	je str2end ; 한 스트링의 입력이 전부 끝났을 때 각각 다른 곳으로 가게 설정한다.
	mov al,str2val
	cmp str1val,al
	ja str1isbig ; str1에 있는 값이 더 클 경우 , str2에 있는 한글자를 출력
	mov al,str1val ; str1 is big 으로 가지 않는 경우 , str1에 있는 값이 더 작다는 것이므로 str1에 있는 한글자 출력
	call writeChar
	inc esi
	jmp goback 

str1isbig: 
	mov al,str2val
	call writeChar
	inc edi
	jmp goback

str1end:
	cmp str2val,0 ; str1의 스트링 입력이 다 끝났을 때 str2로부터 스트링을 받음
	je bothend
	mov al,str2val
	call writeChar
	inc edi
	jmp goback

str2end: ; str2의 스트링 입력이 다 끝났을 때 str1로부터 스트링을 받음
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