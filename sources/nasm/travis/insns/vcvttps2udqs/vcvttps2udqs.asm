default rel
	vcvttps2udqs xmm7, xmm0
	vcvttps2udqs xmm7, xmm3
	vcvttps2udqs ymm1, ymm3
	vcvttps2udqs ymm5, ymm4
	vcvttps2udqs zmm0, zmm5
	vcvttps2udqs zmm6, zmm3
	vcvttps2udqs xmm9, xmm8
	vcvttps2udqs ymm8, ymm12
	vcvttps2udqs zmm12, zmm9
	vcvttps2udqs xmm19, xmm17
	vcvttps2udqs ymm31, ymm26
	vcvttps2udqs zmm27, zmm20
	vcvttps2udqs xmm6{k5}, xmm0
	vcvttps2udqs ymm4{k1}, ymm7
	vcvttps2udqs zmm6{k2}, zword [0x4e6]
	vcvttps2udqs xmm3{k2}{z}, xmm3
	vcvttps2udqs ymm6{k3}{z}, ymm1
	vcvttps2udqs zmm0{k5}{z}, zword [0xc7f]
	vcvttps2udqs zmm6, zmm2, {sae}
	vcvttps2udqs xmm5, oword [eax+1]
	vcvttps2udqs xmm5, oword [eax+64]
	vcvttps2udqs ymm4, yword [eax+1]
	vcvttps2udqs ymm6, yword [eax+64]
	vcvttps2udqs zmm3, zword [eax+1]
	vcvttps2udqs zmm5, zword [eax+64]
	vcvttps2udqs xmm2, [0x9ff]
	vcvttps2udqs ymm3, [0x8ec]
	vcvttps2udqs zmm2, [0x5f8]
