default rel
	vptestnmw k7, xmm0, xmm0
	vptestnmw k7, xmm3, oword [0x6a6]
	vptestnmw k6, ymm7, ymm7
	vptestnmw k1, ymm2, ymm1
	vptestnmw k4, zmm0, zword [0xc3f]
	vptestnmw k6, zmm0, zmm5
	vptestnmw k3, xmm14, xmm14
	vptestnmw k4, ymm10, ymm13
	vptestnmw k1, zmm11, zmm14
	vptestnmw k7, xmm31, xmm28
	vptestnmw k5, ymm31, ymm29
	vptestnmw k6, zmm16, zmm23
	vptestnmw k5{k7}, xmm4, xmm3
	vptestnmw k1{k6}, ymm4, yword [0x21b]
	vptestnmw k2{k3}, zmm4, zword [0x96b]
	vptestnmw k5, xmm6, oword [eax+1]
	vptestnmw k4, xmm0, oword [eax+64]
	vptestnmw k7, ymm0, yword [eax+1]
	vptestnmw k6, ymm0, yword [eax+64]
	vptestnmw k2, zmm4, zword [eax+1]
	vptestnmw k4, zmm4, zword [eax+64]
	vptestnmw k1, xmm6, [0x858]
	vptestnmw k3, ymm5, [0xa15]
	vptestnmw k3, zmm7, [0x785]
