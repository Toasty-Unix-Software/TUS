	pshufb mm4, qword [0x5d4]
	pshufb mm6, qword [0x626]
	pshufb xmm1, oword [0xe64]
	pshufb xmm1, oword [0x24c]

%ifdef ERROR
	pshufb xmm8, xmm10
%endif
