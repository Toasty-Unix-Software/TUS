default rel
	vpmaxsd xmm5, xmm5, xmm4
	vpmaxsd xmm5, xmm4
	vpmaxsd xmm7, xmm6, xmm3
	vpmaxsd ymm3, ymm6, ymm1
	vpmaxsd ymm3, ymm1
	vpmaxsd ymm6, ymm5, ymm0
	vpmaxsd xmm0, xmm5, xmm4
	vpmaxsd xmm0, xmm4
	vpmaxsd xmm3, xmm3, xmm2
	vpmaxsd ymm6, ymm3, yword [0x2b9]
	vpmaxsd ymm6, yword [0x2b9]
	vpmaxsd ymm2, ymm1, yword [0xd84]
	vpmaxsd xmm11, xmm14, xmm14
	vpmaxsd ymm8, ymm14, ymm12
	vpmaxsd xmm10, xmm14, xmm9
	vpmaxsd ymm11, ymm14, ymm15
	vpmaxsd xmm27, xmm19, xmm22
	vpmaxsd ymm24, ymm18, ymm20
	vpmaxsd xmm21, xmm26, xmm21
	vpmaxsd ymm18, ymm30, ymm17
	vpmaxsd xmm3{k7}, xmm6, oword [0x342]
	vpmaxsd ymm7{k7}, ymm3, ymm0
	vpmaxsd zmm0{k3}, zmm1, zmm3
	vpmaxsd xmm5{k5}{z}, xmm0, xmm3
	vpmaxsd ymm4{k1}{z}, ymm7, yword [0xa70]
	vpmaxsd zmm4{k7}{z}, zmm2, zmm7
	vpmaxsd xmm1, xmm3, oword [eax+1]
	vpmaxsd xmm7, xmm3, oword [eax+64]
	vpmaxsd ymm0, ymm2, yword [eax+1]
	vpmaxsd ymm7, ymm2, yword [eax+64]
	vpmaxsd xmm1, xmm6, oword [eax+1]
	vpmaxsd xmm5, xmm3, oword [eax+64]
	vpmaxsd ymm5, ymm1, yword [eax+1]
	vpmaxsd ymm1, ymm2, yword [eax+64]
	vpmaxsd zmm6, zmm3, zword [eax+1]
	vpmaxsd zmm7, zmm5, zword [eax+64]
	vpmaxsd xmm6, xmm3, [0xdfd]
	vpmaxsd ymm3, ymm6, [0xc5e]
	vpmaxsd xmm1, xmm3, [0xdd0]
	vpmaxsd ymm5, ymm0, [0xb56]
	vpmaxsd zmm7, zmm1, [0xc08]
