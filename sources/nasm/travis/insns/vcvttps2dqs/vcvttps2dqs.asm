default rel
	vcvttps2dqs xmm7, xmm7
	vcvttps2dqs xmm3, xmm6
	vcvttps2dqs ymm6, ymm1
	vcvttps2dqs ymm6, yword [0xfd9]
	vcvttps2dqs zmm3, zmm3
	vcvttps2dqs zmm0, zmm4
	vcvttps2dqs xmm11, xmm13
	vcvttps2dqs ymm13, ymm8
	vcvttps2dqs zmm9, zmm10
	vcvttps2dqs xmm23, xmm18
	vcvttps2dqs ymm20, ymm26
	vcvttps2dqs zmm19, zmm19
	vcvttps2dqs xmm5{k4}, xmm5
	vcvttps2dqs ymm2{k1}, ymm0
	vcvttps2dqs zmm7{k5}, zmm6
	vcvttps2dqs xmm2{k3}{z}, xmm2
	vcvttps2dqs ymm1{k2}{z}, yword [0xad7]
	vcvttps2dqs zmm4{k3}{z}, zmm7
	vcvttps2dqs zmm4, zmm3, {sae}
	vcvttps2dqs xmm7, oword [eax+1]
	vcvttps2dqs xmm6, oword [eax+64]
	vcvttps2dqs ymm4, yword [eax+1]
	vcvttps2dqs ymm4, yword [eax+64]
	vcvttps2dqs zmm7, zword [eax+1]
	vcvttps2dqs zmm2, zword [eax+64]
	vcvttps2dqs xmm4, [0xd1a]
	vcvttps2dqs ymm6, [0x312]
	vcvttps2dqs zmm1, [0xc40]
