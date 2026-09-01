	vpopcntw xmm7, xmm6
	vpopcntw xmm6, oword [0xc40]
	vpopcntw ymm5, ymm0
	vpopcntw ymm1, ymm0
	vpopcntw zmm7, zmm0
	vpopcntw zmm2, zmm7

%ifdef ERROR
	vpopcntw xmm13, xmm14
	vpopcntw ymm9, ymm15
	vpopcntw zmm13, zmm14
	vpopcntw xmm28, xmm29
	vpopcntw ymm24, ymm21
	vpopcntw zmm17, zmm31
%endif
