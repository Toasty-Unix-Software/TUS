default rel
	vptestnmb k3, xmm4, oword [0x114]
	vptestnmb k5, xmm1, oword [0xd0a]
	vptestnmb k1, ymm7, ymm4
	vptestnmb k3, ymm4, yword [0x9fd]
	vptestnmb k6, zmm4, zmm7
	vptestnmb k4, zmm3, zword [0x9b7]
	vptestnmb k1, xmm13, xmm12
	vptestnmb k5, ymm10, ymm9
	vptestnmb k5, zmm12, zmm14
	vptestnmb k4, xmm30, xmm25
	vptestnmb k4, ymm20, ymm16
	vptestnmb k2, zmm20, zmm24
	vptestnmb k1{k7}, xmm2, xmm1
	vptestnmb k5{k7}, ymm1, ymm7
	vptestnmb k1{k3}, zmm2, zword [0x302]
	vptestnmb k4, xmm5, oword [eax+1]
	vptestnmb k4, xmm7, oword [eax+64]
	vptestnmb k7, ymm3, yword [eax+1]
	vptestnmb k2, ymm5, yword [eax+64]
	vptestnmb k5, zmm2, zword [eax+1]
	vptestnmb k6, zmm0, zword [eax+64]
	vptestnmb k4, xmm1, [0x56f]
	vptestnmb k6, ymm7, [0x9a4]
	vptestnmb k7, zmm3, [0xa5f]
