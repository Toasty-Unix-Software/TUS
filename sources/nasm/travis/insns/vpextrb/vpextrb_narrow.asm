	vpextrb eax, xmm7, 0xcd
	vpextrb edx, xmm5, 0x34
	vpextrb byte [0x1f1], xmm7, 0x8a
	vpextrb byte [0x846], xmm0, 0xe2
	vpextrb bl, xmm7, 0xec
	vpextrb bl, xmm6, 0xc5

%ifdef ERROR
	vpextrb rbx, xmm7, 0x84
	vpextrb rcx, xmm3, 0x48
	vpextrb r13, xmm8, 0x7b
	vpextrb r10d, xmm13, 0xe9
	vpextrb byte [0x695], xmm11, 0x6b
	vpextrb r12b, xmm15, 0xa9
	vpextrb r27, xmm26, 0x6b
	vpextrb r22d, xmm22, 0x98
	vpextrb byte [0x42b], xmm25, 0xbd
	vpextrb r26b, xmm21, 0x2f
%endif
