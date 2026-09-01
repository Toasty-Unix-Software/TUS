default rel
	vptestmq k4, xmm0, xmm6
	vptestmq k1, xmm3, oword [0xeb8]
	vptestmq k7, ymm5, ymm3
	vptestmq k7, ymm5, ymm0
	vptestmq k4, zmm7, zmm7
	vptestmq k6, zmm7, zmm5
	vptestmq k6, xmm10, xmm15
	vptestmq k4, ymm15, ymm15
	vptestmq k7, zmm14, zmm8
	vptestmq k3, xmm17, xmm17
	vptestmq k6, ymm18, ymm17
	vptestmq k1, zmm31, zmm24
	vptestmq k2{k2}, xmm5, xmm5
	vptestmq k5{k3}, ymm6, ymm7
	vptestmq k2{k5}, zmm2, zword [0x373]
	vptestmq k7, xmm2, oword [eax+1]
	vptestmq k3, xmm2, oword [eax+64]
	vptestmq k2, ymm7, yword [eax+1]
	vptestmq k1, ymm0, yword [eax+64]
	vptestmq k3, zmm0, zword [eax+1]
	vptestmq k2, zmm7, zword [eax+64]
	vptestmq k5, xmm4, [0x2e6]
	vptestmq k3, ymm5, [0xf65]
	vptestmq k4, zmm1, [0x74f]
