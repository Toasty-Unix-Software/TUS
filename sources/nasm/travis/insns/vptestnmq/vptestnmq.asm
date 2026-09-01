default rel
	vptestnmq k5, xmm2, oword [0x889]
	vptestnmq k3, xmm0, oword [0xcae]
	vptestnmq k4, ymm1, ymm5
	vptestnmq k3, ymm3, ymm1
	vptestnmq k5, zmm1, zmm1
	vptestnmq k5, zmm7, zmm7
	vptestnmq k3, xmm13, xmm9
	vptestnmq k3, ymm9, ymm10
	vptestnmq k4, zmm14, zmm14
	vptestnmq k7, xmm19, xmm21
	vptestnmq k4, ymm16, ymm28
	vptestnmq k4, zmm30, zmm19
	vptestnmq k7{k4}, xmm5, xmm3
	vptestnmq k1{k7}, ymm7, yword [0xc45]
	vptestnmq k4{k7}, zmm3, zmm7
	vptestnmq k1, xmm3, oword [eax+1]
	vptestnmq k5, xmm1, oword [eax+64]
	vptestnmq k4, ymm5, yword [eax+1]
	vptestnmq k2, ymm4, yword [eax+64]
	vptestnmq k7, zmm1, zword [eax+1]
	vptestnmq k3, zmm2, zword [eax+64]
	vptestnmq k1, xmm7, [0xcbb]
	vptestnmq k6, ymm1, [0x35e]
	vptestnmq k3, zmm0, [0x193]
