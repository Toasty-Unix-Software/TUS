default rel
	vpmaxuq xmm4, xmm0, oword [0xcf5]
	vpmaxuq xmm4, oword [0xcf5]
	vpmaxuq xmm5, xmm3, xmm5
	vpmaxuq ymm5, ymm6, yword [0x46d]
	vpmaxuq ymm5, yword [0x46d]
	vpmaxuq ymm7, ymm3, ymm1
	vpmaxuq zmm7, zmm0, zmm6
	vpmaxuq zmm7, zmm6
	vpmaxuq zmm3, zmm6, zmm3
	vpmaxuq xmm9, xmm11, xmm12
	vpmaxuq ymm11, ymm9, ymm12
	vpmaxuq zmm13, zmm15, zmm15
	vpmaxuq xmm19, xmm29, xmm23
	vpmaxuq ymm20, ymm29, ymm22
	vpmaxuq zmm18, zmm16, zmm20
	vpmaxuq xmm3{k6}, xmm1, xmm7
	vpmaxuq ymm3{k6}, ymm6, yword [0x47a]
	vpmaxuq zmm6{k4}, zmm6, zword [0x222]
	vpmaxuq xmm3{k4}{z}, xmm5, xmm3
	vpmaxuq ymm0{k3}{z}, ymm6, ymm5
	vpmaxuq zmm6{k5}{z}, zmm0, zmm5
	vpmaxuq xmm0, xmm1, oword [eax+1]
	vpmaxuq xmm3, xmm4, oword [eax+64]
	vpmaxuq ymm0, ymm3, yword [eax+1]
	vpmaxuq ymm1, ymm2, yword [eax+64]
	vpmaxuq zmm6, zmm7, zword [eax+1]
	vpmaxuq zmm0, zmm6, zword [eax+64]
	vpmaxuq xmm7, xmm4, [0x2d1]
	vpmaxuq ymm2, ymm7, [0x1e6]
	vpmaxuq zmm3, zmm3, [0x2f5]
