	vrsqrt28ss xmm0, xmm5, dword [0x54e]
	vrsqrt28ss xmm0, dword [0x54e]
	vrsqrt28ss xmm3, xmm3, xmm4

%ifdef ERROR
	vrsqrt28ss xmm12, xmm12, xmm11
	vrsqrt28ss xmm26, xmm16, xmm29
%endif
