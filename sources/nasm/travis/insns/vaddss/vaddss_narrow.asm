	vaddss xmm2, xmm1, xmm0
	vaddss xmm2, xmm0
	vaddss xmm5, xmm3, xmm3
	vaddss xmm3, xmm5, dword [0xe2e]
	vaddss xmm3, dword [0xe2e]
	vaddss xmm5, xmm0, xmm3

%ifdef ERROR
	vaddss xmm9, xmm12, xmm11
	vaddss xmm13, xmm9, xmm15
	vaddss xmm25, xmm17, xmm22
	vaddss xmm24, xmm16, xmm27
%endif
