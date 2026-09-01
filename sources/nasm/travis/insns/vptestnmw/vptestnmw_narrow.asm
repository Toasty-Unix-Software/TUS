	vptestnmw k7, xmm0, xmm0
	vptestnmw k7, xmm3, oword [0x6a6]
	vptestnmw k6, ymm7, ymm7
	vptestnmw k1, ymm2, ymm1
	vptestnmw k4, zmm0, zword [0xc3f]
	vptestnmw k6, zmm0, zmm5

%ifdef ERROR
	vptestnmw k3, xmm14, xmm14
	vptestnmw k4, ymm10, ymm13
	vptestnmw k1, zmm11, zmm14
	vptestnmw k7, xmm31, xmm28
	vptestnmw k5, ymm31, ymm29
	vptestnmw k6, zmm16, zmm23
%endif
