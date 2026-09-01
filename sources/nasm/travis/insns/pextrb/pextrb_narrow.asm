	pextrb ebx, xmm0, 0xd
	pextrb edi, xmm4, 0x3b
	pextrb byte [0xeb2], xmm1, 0xdf
	pextrb byte [0xee2], xmm4, 0xcd

%ifdef ERROR
	pextrb rsi, xmm2, 0x4d
	pextrb rax, xmm4, 0x82
	pextrb r8d, xmm10, 0x3b
	pextrb byte [0x1cb], xmm11, 0x25
	pextrb r9, xmm15, 0xb8
%endif
