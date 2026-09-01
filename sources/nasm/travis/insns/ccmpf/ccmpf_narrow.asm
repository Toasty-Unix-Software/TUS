	ccmpf 2, al, al
	ccmpf 15, bl, dl
	ccmpf 15, cx, bx
	ccmpf 3, si, bp
	ccmpf 0, dword [0x4ee], ebx
	ccmpf 4, edi, ebp

%ifdef ERROR
	ccmpf 6, rbx, rsi
	ccmpf 1, rdx, rbp
	ccmpf 7, r14b, r9b
	ccmpf 12, r13w, r10w
	ccmpf 7, r8d, r8d
	ccmpf 6, r14, r11
	ccmpf 9, r26b, r28b
	ccmpf 4, r21w, r18w
	ccmpf 0, r21d, r20d
	ccmpf 14, r17, r22
	ccmpf 0, qword [eax+1], rbp
	ccmpf 14, qword [eax+64], rdx
	ccmpf 9, rax, qword [eax+1]
	ccmpf 1, rdx, qword [eax+64]
	ccmpf 10, qword [eax+1], -87
	ccmpf 14, qword [eax+64], 88
	ccmpf 9, qword [eax+1], -87097222
	ccmpf 8, qword [eax+64], 286679442
	ccmpf 13, [0x9a0], rbp
	ccmpf 1, rbp, [0xdf1]
	ccmpf 5, [0xc29], -106
	ccmpf 15, [0x100], -432323728
%endif
