	vrsqrt28ps zmm4, zmm1
	vrsqrt28ps zmm3, zword [0xec9]

%ifdef ERROR
	vrsqrt28ps zmm15, zmm8
	vrsqrt28ps zmm16, zmm27
%endif
