default rel
	vcvtph2ibs xmm6, xmm2
	vcvtph2ibs xmm1, xmm4
	vcvtph2ibs ymm5, yword [0xded]
	vcvtph2ibs ymm6, ymm1
	vcvtph2ibs zmm4, zword [0x81d]
	vcvtph2ibs zmm1, zmm5
	vcvtph2ibs xmm13, xmm11
	vcvtph2ibs ymm13, ymm14
	vcvtph2ibs zmm14, zmm14
	vcvtph2ibs xmm31, xmm23
	vcvtph2ibs ymm20, ymm26
	vcvtph2ibs zmm20, zmm24
	vcvtph2ibs xmm3{k7}, xmm1
	vcvtph2ibs ymm3{k5}, yword [0x548]
	vcvtph2ibs zmm3{k2}, zmm6
	vcvtph2ibs xmm4{k1}{z}, xmm6
	vcvtph2ibs ymm5{k6}{z}, yword [0x67c]
	vcvtph2ibs zmm4{k5}{z}, zword [0x835]
	vcvtph2ibs zmm7, zmm5, {rd-sae}
	vcvtph2ibs xmm6, oword [eax+1]
	vcvtph2ibs xmm3, oword [eax+64]
	vcvtph2ibs ymm6, yword [eax+1]
	vcvtph2ibs ymm4, yword [eax+64]
	vcvtph2ibs zmm7, zword [eax+1]
	vcvtph2ibs zmm3, zword [eax+64]
	vcvtph2ibs xmm5, [0xe8e]
	vcvtph2ibs ymm3, [0x355]
	vcvtph2ibs zmm6, [0xdc3]
