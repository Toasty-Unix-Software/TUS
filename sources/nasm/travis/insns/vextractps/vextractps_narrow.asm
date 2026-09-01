	vextractps ecx, xmm1, 0x59
	vextractps ebp, xmm5, 0x1
	vextractps esi, xmm0, 0xf0
	vextractps eax, xmm4, 0x3e
	vextractps dword [0xbf8], xmm3, 0xb8
	vextractps dword [0x172], xmm1, 0xe1

%ifdef ERROR
	vextractps rbp, xmm1, 0x27
	vextractps rsi, xmm4, 0x50
	vextractps r15d, xmm12, 0x1a
	vextractps r14d, xmm9, 0xba
	vextractps r15, xmm14, 0x43
	vextractps dword [0x5a2], xmm14, 0x35
	vextractps r28d, xmm24, 0x30
	vextractps r17d, xmm23, 0x66
	vextractps r25, xmm27, 0xd6
	vextractps dword [0x728], xmm27, 0x63
%endif
