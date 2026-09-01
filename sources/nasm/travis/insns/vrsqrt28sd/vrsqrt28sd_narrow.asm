	vrsqrt28sd xmm0, xmm0, xmm0
	vrsqrt28sd xmm0, xmm0
	vrsqrt28sd xmm2, xmm3, xmm4

%ifdef ERROR
	vrsqrt28sd xmm11, xmm10, xmm12
	vrsqrt28sd xmm27, xmm27, xmm25
%endif
