	ccmple 4, bl, cl
	ccmple 1, byte [0x1e4], dl
	ccmple 5, di, cx
	ccmple 14, dx, bp
	ccmple 12, ebp, ecx
	ccmple 6, dword [0xda2], ebp

%ifdef ERROR
	ccmple 14, rsi, rbp
	ccmple 11, rdi, rcx
	ccmple 3, r9b, r12b
	ccmple 15, r13w, r12w
	ccmple 9, r11d, r13d
	ccmple 15, r8, r9
	ccmple 0, r26b, r25b
	ccmple 3, r21w, r22w
	ccmple 15, r28d, r22d
	ccmple 12, r24, r27
	ccmple 9, qword [eax+1], rbp
	ccmple 7, qword [eax+64], rsi
	ccmple 9, rdx, qword [eax+1]
	ccmple 15, rax, qword [eax+64]
	ccmple 0, qword [eax+1], 95
	ccmple 4, qword [eax+64], -14
	ccmple 6, qword [eax+1], 46608980
	ccmple 5, qword [eax+64], -267493611
	ccmple 2, [0xf76], rsi
	ccmple 15, rdi, [0xa96]
	ccmple 2, [0x39a], 83
	ccmple 13, [0x7b3], -508976809
%endif
