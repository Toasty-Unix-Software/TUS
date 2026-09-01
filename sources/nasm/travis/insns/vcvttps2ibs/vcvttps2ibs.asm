default rel
	vcvttps2ibs xmm0, oword [0x657]
	vcvttps2ibs xmm3, oword [0x61a]
	vcvttps2ibs ymm3, ymm1
	vcvttps2ibs ymm7, ymm4
	vcvttps2ibs zmm5, zword [0xc95]
	vcvttps2ibs zmm0, zmm5
	vcvttps2ibs xmm8, xmm11
	vcvttps2ibs ymm9, ymm14
	vcvttps2ibs zmm11, zmm9
	vcvttps2ibs xmm17, xmm25
	vcvttps2ibs ymm20, ymm29
	vcvttps2ibs zmm31, zmm19
	vcvttps2ibs xmm4{k4}, xmm0
	vcvttps2ibs ymm6{k6}, ymm0
	vcvttps2ibs zmm1{k7}, zmm3
	vcvttps2ibs xmm7{k2}{z}, xmm5
	vcvttps2ibs ymm1{k2}{z}, yword [0xf46]
	vcvttps2ibs zmm4{k4}{z}, zmm5
	vcvttps2ibs zmm5, zmm5, {sae}
	vcvttps2ibs xmm6, oword [eax+1]
	vcvttps2ibs xmm2, oword [eax+64]
	vcvttps2ibs ymm1, yword [eax+1]
	vcvttps2ibs ymm7, yword [eax+64]
	vcvttps2ibs zmm5, zword [eax+1]
	vcvttps2ibs zmm0, zword [eax+64]
	vcvttps2ibs xmm4, [0xcbf]
	vcvttps2ibs ymm5, [0x888]
	vcvttps2ibs zmm1, [0xf7b]
