default rel
	vcvttps2uqqs xmm7, qword [0xf2a]
	vcvttps2uqqs xmm2, qword [0x799]
	vcvttps2uqqs ymm2, oword [0xbed]
	vcvttps2uqqs ymm2, xmm1
	vcvttps2uqqs zmm4, ymm7
	vcvttps2uqqs zmm2, ymm1
	vcvttps2uqqs xmm10, xmm12
	vcvttps2uqqs ymm15, xmm14
	vcvttps2uqqs zmm12, ymm11
	vcvttps2uqqs xmm23, xmm30
	vcvttps2uqqs ymm26, xmm21
	vcvttps2uqqs zmm24, ymm25
	vcvttps2uqqs xmm2{k3}, xmm6
	vcvttps2uqqs ymm6{k3}, xmm4
	vcvttps2uqqs zmm4{k1}, yword [0x831]
	vcvttps2uqqs xmm6{k7}{z}, xmm7
	vcvttps2uqqs ymm4{k1}{z}, oword [0xdb7]
	vcvttps2uqqs zmm0{k2}{z}, ymm1
	vcvttps2uqqs zmm0, ymm3, {sae}
	vcvttps2uqqs xmm5, qword [eax+1]
	vcvttps2uqqs xmm3, qword [eax+64]
	vcvttps2uqqs ymm1, oword [eax+1]
	vcvttps2uqqs ymm6, oword [eax+64]
	vcvttps2uqqs zmm7, yword [eax+1]
	vcvttps2uqqs zmm2, yword [eax+64]
	vcvttps2uqqs xmm6, [0x340]
	vcvttps2uqqs ymm6, [0x605]
	vcvttps2uqqs zmm4, [0x850]
