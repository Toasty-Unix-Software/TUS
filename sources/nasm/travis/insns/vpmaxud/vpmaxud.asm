default rel
	vpmaxud xmm7, xmm4, oword [0x967]
	vpmaxud xmm7, oword [0x967]
	vpmaxud xmm5, xmm1, xmm5
	vpmaxud ymm6, ymm1, ymm6
	vpmaxud ymm6, ymm6
	vpmaxud ymm5, ymm6, ymm7
	vpmaxud xmm2, xmm1, xmm3
	vpmaxud xmm2, xmm3
	vpmaxud xmm1, xmm3, xmm4
	vpmaxud ymm6, ymm6, ymm1
	vpmaxud ymm6, ymm1
	vpmaxud ymm5, ymm7, yword [0x28d]
	vpmaxud xmm8, xmm13, xmm14
	vpmaxud ymm12, ymm15, ymm9
	vpmaxud xmm14, xmm9, xmm15
	vpmaxud ymm11, ymm12, ymm9
	vpmaxud xmm27, xmm19, xmm31
	vpmaxud ymm22, ymm28, ymm30
	vpmaxud xmm23, xmm20, xmm26
	vpmaxud ymm26, ymm30, ymm19
	vpmaxud xmm2{k3}, xmm6, xmm5
	vpmaxud ymm2{k1}, ymm5, ymm4
	vpmaxud zmm4{k5}, zmm6, zmm7
	vpmaxud xmm0{k4}{z}, xmm2, xmm4
	vpmaxud ymm2{k6}{z}, ymm1, ymm0
	vpmaxud zmm4{k3}{z}, zmm1, zmm4
	vpmaxud xmm1, xmm1, oword [eax+1]
	vpmaxud xmm3, xmm4, oword [eax+64]
	vpmaxud ymm1, ymm7, yword [eax+1]
	vpmaxud ymm3, ymm7, yword [eax+64]
	vpmaxud xmm0, xmm6, oword [eax+1]
	vpmaxud xmm7, xmm3, oword [eax+64]
	vpmaxud ymm1, ymm2, yword [eax+1]
	vpmaxud ymm2, ymm2, yword [eax+64]
	vpmaxud zmm0, zmm3, zword [eax+1]
	vpmaxud zmm1, zmm1, zword [eax+64]
	vpmaxud xmm7, xmm4, [0x14d]
	vpmaxud ymm2, ymm1, [0xbcb]
	vpmaxud xmm5, xmm5, [0x164]
	vpmaxud ymm0, ymm5, [0x110]
	vpmaxud zmm4, zmm0, [0x7a3]
