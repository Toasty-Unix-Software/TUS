default rel
	vpminsq xmm4, xmm1, xmm1
	vpminsq xmm4, xmm1
	vpminsq xmm4, xmm5, xmm6
	vpminsq ymm6, ymm0, yword [0x89b]
	vpminsq ymm6, yword [0x89b]
	vpminsq ymm3, ymm4, ymm1
	vpminsq zmm2, zmm6, zword [0x61c]
	vpminsq zmm2, zword [0x61c]
	vpminsq zmm2, zmm4, zmm1
	vpminsq xmm13, xmm14, xmm12
	vpminsq ymm13, ymm15, ymm13
	vpminsq zmm9, zmm13, zmm12
	vpminsq xmm18, xmm17, xmm28
	vpminsq ymm21, ymm21, ymm20
	vpminsq zmm29, zmm17, zmm19
	vpminsq xmm2{k2}, xmm4, oword [0x423]
	vpminsq ymm2{k1}, ymm4, ymm0
	vpminsq zmm3{k2}, zmm6, zmm2
	vpminsq xmm6{k4}{z}, xmm5, xmm5
	vpminsq ymm6{k7}{z}, ymm0, ymm3
	vpminsq zmm0{k5}{z}, zmm1, zmm0
	vpminsq xmm0, xmm1, oword [eax+1]
	vpminsq xmm6, xmm5, oword [eax+64]
	vpminsq ymm2, ymm6, yword [eax+1]
	vpminsq ymm4, ymm3, yword [eax+64]
	vpminsq zmm7, zmm4, zword [eax+1]
	vpminsq zmm7, zmm1, zword [eax+64]
	vpminsq xmm3, xmm4, [0xce2]
	vpminsq ymm4, ymm0, [0x4d1]
	vpminsq zmm4, zmm0, [0xc49]
