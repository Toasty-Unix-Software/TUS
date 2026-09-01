	movsxw eax, ax
	movsxw eax, ax
	movsxw ax, cx
	movsxw cx, si
	movsxw edi, word [0x190]
	movsxw esi, ax

%ifdef ERROR
	movsxw rdi, word [0xef4]
	movsxw rbp, dx
	movsxw r8w, r8w
	movsxw r11d, r13w
	movsxw r11, r9w
	movsxw r17w, r23w
	movsxw r16d, r18w
	movsxw r29, r28w
	movsxw rsi, word [eax+1]
	movsxw rbp, word [eax+64]
	movsxw rcx, [0x32d]
%endif
