	vrsqrtss xmm1, xmm4, dword [0xeda]
	vrsqrtss xmm1, dword [0xeda]
	vrsqrtss xmm3, xmm1, xmm5

%ifdef ERROR
	vrsqrtss xmm13, xmm8, xmm15
%endif
