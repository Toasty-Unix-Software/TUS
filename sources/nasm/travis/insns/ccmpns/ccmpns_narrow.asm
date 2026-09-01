	ccmpns 11, byte [0x13f], al
	ccmpns 5, al, al
	ccmpns 2, di, cx
	ccmpns 6, word [0xa3e], si
	ccmpns 1, dword [0xabb], ebp
	ccmpns 0, ecx, edi

%ifdef ERROR
	ccmpns 5, rcx, rcx
	ccmpns 0, rbp, rdx
	ccmpns 0, r13b, r12b
	ccmpns 3, r11w, r9w
	ccmpns 6, r13d, r11d
	ccmpns 0, r11, r8
	ccmpns 3, r29b, r29b
	ccmpns 1, r20w, r24w
	ccmpns 0, r21d, r26d
	ccmpns 5, r29, r28
	ccmpns 9, qword [eax+1], rbx
	ccmpns 1, qword [eax+64], rsi
	ccmpns 8, rdx, qword [eax+1]
	ccmpns 15, rdi, qword [eax+64]
	ccmpns 7, qword [eax+1], 124
	ccmpns 11, qword [eax+64], 58
	ccmpns 3, qword [eax+1], -52534558
	ccmpns 0, qword [eax+64], -503518367
	ccmpns 13, [0xf5d], rsi
	ccmpns 14, rcx, [0x2ec]
	ccmpns 12, [0xf7e], -97
	ccmpns 13, [0x282], 347917985
%endif
