default rel
	vpsravq xmm4, xmm5, xmm7
	vpsravq xmm4, xmm7
	vpsravq xmm4, xmm0, xmm0
	vpsravq ymm3, ymm2, ymm5
	vpsravq ymm3, ymm5
	vpsravq ymm2, ymm5, ymm5
	vpsravq zmm3, zmm4, zmm0
	vpsravq zmm3, zmm0
	vpsravq zmm5, zmm7, zword [0x9be]
	vpsravq xmm13, xmm15, xmm9
	vpsravq ymm9, ymm14, ymm11
	vpsravq zmm15, zmm12, zmm12
	vpsravq xmm25, xmm29, xmm25
	vpsravq ymm28, ymm22, ymm31
	vpsravq zmm30, zmm23, zmm31
	vpsravq xmm4{k7}, xmm2, oword [0x274]
	vpsravq ymm3{k3}, ymm3, ymm2
	vpsravq zmm4{k4}, zmm4, zmm5
	vpsravq xmm0{k6}{z}, xmm2, xmm0
	vpsravq ymm4{k4}{z}, ymm2, ymm2
	vpsravq zmm3{k1}{z}, zmm0, zmm4
	vpsravq xmm3, xmm3, oword [eax+1]
	vpsravq xmm4, xmm1, oword [eax+64]
	vpsravq ymm7, ymm2, yword [eax+1]
	vpsravq ymm1, ymm1, yword [eax+64]
	vpsravq zmm0, zmm2, zword [eax+1]
	vpsravq zmm7, zmm3, zword [eax+64]
	vpsravq xmm4, xmm2, [0x3b1]
	vpsravq ymm0, ymm5, [0x8db]
	vpsravq zmm0, zmm0, [0x9ae]
