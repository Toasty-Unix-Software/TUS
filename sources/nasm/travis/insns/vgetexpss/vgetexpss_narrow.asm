	vgetexpss xmm4, xmm3, dword [0xed3]
	vgetexpss xmm3, xmm0, xmm5

%ifdef ERROR
	vgetexpss xmm11, xmm15, xmm13
	vgetexpss xmm21, xmm21, xmm16
%endif
