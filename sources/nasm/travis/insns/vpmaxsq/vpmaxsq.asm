default rel
	vpmaxsq xmm7, xmm1, oword [0xb1c]
	vpmaxsq xmm7, oword [0xb1c]
	vpmaxsq xmm5, xmm1, xmm5
	vpmaxsq ymm4, ymm4, ymm5
	vpmaxsq ymm4, ymm5
	vpmaxsq ymm7, ymm2, ymm7
	vpmaxsq zmm6, zmm1, zmm2
	vpmaxsq zmm6, zmm2
	vpmaxsq zmm5, zmm3, zword [0x14b]
	vpmaxsq xmm12, xmm9, xmm8
	vpmaxsq ymm12, ymm13, ymm15
	vpmaxsq zmm14, zmm14, zmm12
	vpmaxsq xmm22, xmm18, xmm19
	vpmaxsq ymm22, ymm16, ymm30
	vpmaxsq zmm23, zmm25, zmm28
	vpmaxsq xmm4{k3}, xmm0, xmm6
	vpmaxsq ymm2{k5}, ymm4, yword [0x60b]
	vpmaxsq zmm1{k6}, zmm1, zword [0x3ac]
	vpmaxsq xmm1{k1}{z}, xmm4, xmm0
	vpmaxsq ymm2{k7}{z}, ymm3, ymm6
	vpmaxsq zmm3{k7}{z}, zmm1, zword [0x24f]
	vpmaxsq xmm6, xmm6, oword [eax+1]
	vpmaxsq xmm2, xmm0, oword [eax+64]
	vpmaxsq ymm2, ymm7, yword [eax+1]
	vpmaxsq ymm5, ymm2, yword [eax+64]
	vpmaxsq zmm3, zmm1, zword [eax+1]
	vpmaxsq zmm0, zmm5, zword [eax+64]
	vpmaxsq xmm5, xmm6, [0xf1d]
	vpmaxsq ymm6, ymm1, [0x4fc]
	vpmaxsq zmm2, zmm1, [0xa10]
