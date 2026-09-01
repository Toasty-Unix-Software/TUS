default rel
	vptestmd k1, xmm2, xmm0
	vptestmd k2, xmm6, xmm7
	vptestmd k2, ymm2, yword [0x1b3]
	vptestmd k7, ymm4, ymm1
	vptestmd k4, zmm1, zmm7
	vptestmd k6, zmm0, zmm4
	vptestmd k1, xmm9, xmm8
	vptestmd k7, ymm15, ymm14
	vptestmd k5, zmm9, zmm13
	vptestmd k1, xmm17, xmm19
	vptestmd k1, ymm22, ymm31
	vptestmd k6, zmm17, zmm16
	vptestmd k6{k7}, xmm3, xmm3
	vptestmd k4{k6}, ymm4, yword [0xda9]
	vptestmd k3{k5}, zmm7, zmm7
	vptestmd k6, xmm3, oword [eax+1]
	vptestmd k3, xmm7, oword [eax+64]
	vptestmd k3, ymm4, yword [eax+1]
	vptestmd k2, ymm6, yword [eax+64]
	vptestmd k7, zmm1, zword [eax+1]
	vptestmd k3, zmm6, zword [eax+64]
	vptestmd k2, xmm6, [0x369]
	vptestmd k3, ymm0, [0x4fa]
	vptestmd k6, zmm4, [0x623]
