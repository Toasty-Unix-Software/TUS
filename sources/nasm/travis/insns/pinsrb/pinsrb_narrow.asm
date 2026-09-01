	pinsrb xmm3, [0x6ba], 0x3a
	pinsrb xmm4, [0xf70], 0xe
	pinsrb xmm7, dl, 0x9
	pinsrb xmm0, dl, 0x26
	pinsrb xmm1, ebp, 0xd7
	pinsrb xmm4, esi, 0x1a

%ifdef ERROR
	pinsrb xmm13, [0x1bf], 0xc5
	pinsrb xmm15, r13b, 0x91
	pinsrb xmm11, r12d, 0x3a
%endif
