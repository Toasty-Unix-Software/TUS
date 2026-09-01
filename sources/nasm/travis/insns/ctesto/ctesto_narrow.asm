	ctesto 9, al, al
	ctesto 12, byte [0x1f0], cl
	ctesto 6, dx, bx
	ctesto 1, di, dx
	ctesto 14, dword [0xec6], ebp
	ctesto 8, esi, ecx

%ifdef ERROR
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
	ctesto 8, qword [eax+1], rax
	ctesto 0, qword [eax+64], rcx
	ctesto 7, qword [eax+1], 65570845
	ctesto 11, qword [eax+64], -381615414
	ctesto 5, [0xff9], rsi
	ctesto 1, [0xd1e], -388813659
%endif
