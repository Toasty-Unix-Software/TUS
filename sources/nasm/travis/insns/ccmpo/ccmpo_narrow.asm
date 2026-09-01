	ccmpo 15, bl, cl
	ccmpo 3, al, al
	ccmpo 3, cx, si
	ccmpo 9, ax, bx
	ccmpo 11, dword [0x2c9], edi
	ccmpo 15, dword [0x7c2], edi

%ifdef ERROR
	ccmpo 14, rcx, rsi
	ccmpo 3, qword [0x920], rbp
	ccmpo 9, r9b, r10b
	ccmpo 13, r13w, r11w
	ccmpo 10, r11d, r11d
	ccmpo 4, r10, r13
	ccmpo 1, r23b, r24b
	ccmpo 0, r26w, r23w
	ccmpo 7, r30d, r21d
	ccmpo 12, r17, r24
	ccmpo 8, qword [eax+1], rax
	ccmpo 1, qword [eax+64], rax
	ccmpo 2, rsi, qword [eax+1]
	ccmpo 5, rax, qword [eax+64]
	ccmpo 0, qword [eax+1], -25
	ccmpo 5, qword [eax+64], -77
	ccmpo 15, qword [eax+1], 439212364
	ccmpo 8, qword [eax+64], 125419640
	ccmpo 13, [0x5f7], rax
	ccmpo 4, rbp, [0xe09]
	ccmpo 8, [0xb31], -90
	ccmpo 3, [0x443], 17233092
%endif
