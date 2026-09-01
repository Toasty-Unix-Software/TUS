default rel
	vptestmw k4, xmm0, oword [0xcd6]
	vptestmw k2, xmm7, xmm3
	vptestmw k3, ymm5, ymm6
	vptestmw k6, ymm2, ymm3
	vptestmw k3, zmm6, zmm7
	vptestmw k1, zmm3, zword [0xa5d]
	vptestmw k3, xmm12, xmm14
	vptestmw k1, ymm15, ymm15
	vptestmw k3, zmm10, zmm14
	vptestmw k4, xmm26, xmm28
	vptestmw k7, ymm24, ymm27
	vptestmw k4, zmm27, zmm31
	vptestmw k3{k6}, xmm4, xmm6
	vptestmw k6{k3}, ymm4, ymm2
	vptestmw k2{k1}, zmm2, zword [0x3c8]
	vptestmw k5, xmm6, oword [eax+1]
	vptestmw k6, xmm0, oword [eax+64]
	vptestmw k4, ymm6, yword [eax+1]
	vptestmw k3, ymm5, yword [eax+64]
	vptestmw k6, zmm3, zword [eax+1]
	vptestmw k1, zmm3, zword [eax+64]
	vptestmw k4, xmm5, [0xf68]
	vptestmw k6, ymm0, [0xf74]
	vptestmw k2, zmm6, [0x592]
