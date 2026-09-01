	vmulss xmm4, xmm2, xmm0
	vmulss xmm4, xmm0
	vmulss xmm5, xmm6, xmm4
	vmulss xmm2, xmm1, dword [0xecc]
	vmulss xmm2, dword [0xecc]
	vmulss xmm0, xmm4, xmm1

%ifdef ERROR
	vmulss xmm8, xmm9, xmm14
	vmulss xmm14, xmm13, xmm14
	vmulss xmm28, xmm29, xmm19
	vmulss xmm30, xmm25, xmm31
%endif
