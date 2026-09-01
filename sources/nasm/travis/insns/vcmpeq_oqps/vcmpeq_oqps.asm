default rel
	vcmpeq_oqps k1, xmm1, xmm7
	vcmpeq_oqps k4, xmm2, xmm6
	vcmpeq_oqps k6, ymm0, ymm6
	vcmpeq_oqps k1, ymm7, ymm5
	vcmpeq_oqps k2, zmm2, zmm1
	vcmpeq_oqps k6, zmm5, zmm7
	vcmpeq_oqps k6, xmm12, xmm10
	vcmpeq_oqps k5, ymm14, ymm9
	vcmpeq_oqps k4, zmm14, zmm10
	vcmpeq_oqps k2, xmm26, xmm27
	vcmpeq_oqps k2, ymm24, ymm17
	vcmpeq_oqps k2, zmm25, zmm30
	vcmpeq_oqps k5{k5}, xmm7, xmm7
	vcmpeq_oqps k2{k3}, ymm3, ymm0
	vcmpeq_oqps k3{k3}, zmm0, zmm6
	vcmpeq_oqps k1, zmm3, zmm2, {sae}
	vcmpeq_oqps k4, xmm0, oword [eax+1]
	vcmpeq_oqps k3, xmm3, oword [eax+64]
	vcmpeq_oqps k7, ymm7, yword [eax+1]
	vcmpeq_oqps k3, ymm2, yword [eax+64]
	vcmpeq_oqps k2, zmm7, zword [eax+1]
	vcmpeq_oqps k7, zmm1, zword [eax+64]
	vcmpeq_oqps k1, xmm6, [0xa7c]
	vcmpeq_oqps k6, ymm3, [0xdc7]
	vcmpeq_oqps k7, zmm3, [0x4ad]
