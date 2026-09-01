default rel
	ctesto 9, al, al
	ctesto 12, byte [0x1f0], cl
	ctesto 6, dx, bx
	ctesto 1, di, dx
	ctesto 14, dword [0xec6], ebp
	ctesto 8, esi, ecx
	ctesto 2, qword [0xe81], rbp
	ctesto 7, qword [0x5d8], rbx
	ctesto 2, r11b, r8b
	ctesto 1, r11w, r8w
	ctesto 0, r14d, r10d
	ctesto 15, r13, r9
	ctesto 1, r18b, r29b
	ctesto 7, r19w, r26w
	ctesto 4, r23d, r21d
	ctesto 7, r18, r31
	ctesto 7, byte [eax+1], bl
	ctesto 0, byte [eax+64], cl
	ctesto 11, word [eax+1], di
	ctesto 13, word [eax+64], cx
	ctesto 0, dword [eax+1], ecx
	ctesto 10, dword [eax+64], ecx
	ctesto 8, qword [eax+1], rax
	ctesto 0, qword [eax+64], rcx
	ctesto 8, byte [eax+1], 0x4c
	ctesto 14, byte [eax+64], 0xee
	ctesto 5, word [eax+1], 0xba17
	ctesto 0, word [eax+64], 0x3388
	ctesto 13, dword [eax+1], 0x39b0655a
	ctesto 5, dword [eax+64], 0x1adc93b8
	ctesto 7, qword [eax+1], 65570845
	ctesto 11, qword [eax+64], -381615414
	ctesto 2, [0xaf8], cl
	ctesto 13, [0xbde], dx
	ctesto 14, [0x107], ecx
	ctesto 5, [0xff9], rsi
	ctesto 7, [0x581], 0x53
	ctesto 6, [0xf77], 0x61de
	ctesto 2, [0x284], 0x1cb251b2
	ctesto 1, [0xd1e], -388813659
