default rel
	vpmuldq xmm7, xmm1, oword [0x84d]
	vpmuldq xmm7, oword [0x84d]
	vpmuldq xmm6, xmm7, xmm1
	vpmuldq ymm7, ymm3, yword [0x674]
	vpmuldq ymm7, yword [0x674]
	vpmuldq ymm3, ymm5, yword [0xf4a]
	vpmuldq xmm6, xmm4, xmm7
	vpmuldq xmm6, xmm7
	vpmuldq xmm7, xmm3, xmm3
	vpmuldq ymm3, ymm6, ymm3
	vpmuldq ymm3, ymm3
	vpmuldq ymm3, ymm3, yword [0x61e]
	vpmuldq xmm12, xmm9, xmm14
	vpmuldq ymm14, ymm13, ymm11
	vpmuldq xmm12, xmm14, xmm15
	vpmuldq ymm15, ymm8, ymm12
	vpmuldq xmm26, xmm25, xmm25
	vpmuldq ymm31, ymm31, ymm17
	vpmuldq xmm22, xmm29, xmm19
	vpmuldq ymm20, ymm17, ymm26
	vpmuldq xmm5{k1}, xmm2, xmm0
	vpmuldq ymm2{k7}, ymm4, yword [0x934]
	vpmuldq zmm0{k5}, zmm1, zmm2
	vpmuldq xmm5{k1}{z}, xmm7, oword [0xbee]
	vpmuldq ymm2{k7}{z}, ymm0, ymm6
	vpmuldq zmm0{k5}{z}, zmm1, zmm4
	vpmuldq xmm5, xmm3, oword [eax+1]
	vpmuldq xmm7, xmm6, oword [eax+64]
	vpmuldq ymm7, ymm0, yword [eax+1]
	vpmuldq ymm1, ymm5, yword [eax+64]
	vpmuldq xmm4, xmm1, oword [eax+1]
	vpmuldq xmm6, xmm2, oword [eax+64]
	vpmuldq ymm6, ymm5, yword [eax+1]
	vpmuldq ymm1, ymm6, yword [eax+64]
	vpmuldq zmm6, zmm6, zword [eax+1]
	vpmuldq zmm3, zmm4, zword [eax+64]
	vpmuldq xmm6, xmm0, [0xea6]
	vpmuldq ymm3, ymm2, [0x169]
	vpmuldq xmm6, xmm5, [0xe27]
	vpmuldq ymm7, ymm7, [0x511]
	vpmuldq zmm5, zmm6, [0xf2d]
