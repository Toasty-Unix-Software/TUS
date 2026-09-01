	vsqrtss xmm4, xmm6, xmm3
	vsqrtss xmm4, xmm3
	vsqrtss xmm4, xmm4, xmm7
	vsqrtss xmm6, xmm7, dword [0x340]
	vsqrtss xmm6, dword [0x340]
	vsqrtss xmm0, xmm0, xmm3

%ifdef ERROR
	vsqrtss xmm13, xmm8, xmm9
	vsqrtss xmm10, xmm8, xmm11
	vsqrtss xmm22, xmm27, xmm25
	vsqrtss xmm21, xmm21, xmm22
%endif
