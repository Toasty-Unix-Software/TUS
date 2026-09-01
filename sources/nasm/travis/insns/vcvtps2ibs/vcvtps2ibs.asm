default rel
	vcvtps2ibs xmm4, xmm7
	vcvtps2ibs xmm2, oword [0xb55]
	vcvtps2ibs ymm7, yword [0xe1d]
	vcvtps2ibs ymm0, ymm5
	vcvtps2ibs zmm2, zmm0
	vcvtps2ibs zmm6, zmm4
	vcvtps2ibs xmm9, xmm9
	vcvtps2ibs ymm12, ymm11
	vcvtps2ibs zmm8, zmm11
	vcvtps2ibs xmm31, xmm25
	vcvtps2ibs ymm29, ymm25
	vcvtps2ibs zmm16, zmm27
	vcvtps2ibs xmm6{k7}, oword [0x291]
	vcvtps2ibs ymm0{k7}, yword [0x504]
	vcvtps2ibs zmm6{k6}, zmm2
	vcvtps2ibs xmm6{k3}{z}, xmm2
	vcvtps2ibs ymm1{k6}{z}, ymm1
	vcvtps2ibs zmm2{k2}{z}, zmm6
	vcvtps2ibs zmm5, zmm4, {rn-sae}
	vcvtps2ibs xmm3, oword [eax+1]
	vcvtps2ibs xmm1, oword [eax+64]
	vcvtps2ibs ymm1, yword [eax+1]
	vcvtps2ibs ymm6, yword [eax+64]
	vcvtps2ibs zmm7, zword [eax+1]
	vcvtps2ibs zmm0, zword [eax+64]
	vcvtps2ibs xmm3, [0x89c]
	vcvtps2ibs ymm2, [0xdb3]
	vcvtps2ibs zmm0, [0xf2f]
