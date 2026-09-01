	vpblendmd xmm7, xmm0, xmm7
	vpblendmd xmm1, xmm3, oword [0xa99]
	vpblendmd ymm6, ymm3, yword [0x2b2]
	vpblendmd ymm5, ymm2, ymm6
	vpblendmd zmm7, zmm6, zmm6
	vpblendmd zmm5, zmm7, zword [0x4ed]

%ifdef ERROR
	vpblendmd xmm10, xmm11, xmm13
	vpblendmd ymm15, ymm14, ymm10
	vpblendmd zmm15, zmm12, zmm15
	vpblendmd xmm20, xmm22, xmm20
	vpblendmd ymm18, ymm16, ymm29
	vpblendmd zmm25, zmm30, zmm21
%endif
