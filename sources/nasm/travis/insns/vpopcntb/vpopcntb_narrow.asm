	vpopcntb xmm7, xmm4
	vpopcntb xmm7, oword [0xc6d]
	vpopcntb ymm5, ymm7
	vpopcntb ymm4, ymm4
	vpopcntb zmm1, zmm7
	vpopcntb zmm5, zmm2

%ifdef ERROR
	vpopcntb xmm15, xmm14
	vpopcntb ymm14, ymm14
	vpopcntb zmm13, zmm14
	vpopcntb xmm20, xmm29
	vpopcntb ymm25, ymm18
	vpopcntb zmm22, zmm26
%endif
