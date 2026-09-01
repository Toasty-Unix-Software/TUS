default rel
	vptestnmd k2, xmm0, xmm2
	vptestnmd k5, xmm5, xmm6
	vptestnmd k1, ymm3, ymm0
	vptestnmd k3, ymm3, ymm2
	vptestnmd k6, zmm7, zmm0
	vptestnmd k2, zmm3, zmm6
	vptestnmd k2, xmm11, xmm9
	vptestnmd k1, ymm14, ymm14
	vptestnmd k1, zmm14, zmm12
	vptestnmd k4, xmm17, xmm27
	vptestnmd k4, ymm27, ymm26
	vptestnmd k3, zmm16, zmm24
	vptestnmd k3{k5}, xmm4, xmm0
	vptestnmd k4{k7}, ymm7, ymm5
	vptestnmd k2{k5}, zmm4, zmm6
	vptestnmd k5, xmm0, oword [eax+1]
	vptestnmd k2, xmm2, oword [eax+64]
	vptestnmd k2, ymm4, yword [eax+1]
	vptestnmd k6, ymm2, yword [eax+64]
	vptestnmd k3, zmm1, zword [eax+1]
	vptestnmd k7, zmm1, zword [eax+64]
	vptestnmd k2, xmm7, [0xe52]
	vptestnmd k2, ymm2, [0x535]
	vptestnmd k6, zmm3, [0xd06]
