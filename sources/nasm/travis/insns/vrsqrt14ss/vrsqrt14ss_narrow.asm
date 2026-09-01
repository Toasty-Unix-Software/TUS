	vrsqrt14ss xmm3, xmm4, xmm3
	vrsqrt14ss xmm3, xmm3
	vrsqrt14ss xmm0, xmm4, dword [0x109]

%ifdef ERROR
	vrsqrt14ss xmm11, xmm12, xmm13
	vrsqrt14ss xmm30, xmm28, xmm20
%endif
