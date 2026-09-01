	vcmptrue_usss xmm3, xmm1, xmm1
	vcmptrue_usss xmm3, xmm1
	vcmptrue_usss xmm4, xmm7, xmm6
	vcmptrue_usss k1, xmm6, dword [0xc9f]
	vcmptrue_usss k4, xmm6, xmm2

%ifdef ERROR
	vcmptrue_usss xmm15, xmm12, xmm8
	vcmptrue_usss k3, xmm14, xmm15
%endif
