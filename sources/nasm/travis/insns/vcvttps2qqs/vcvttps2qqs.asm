default rel
	vcvttps2qqs xmm0, xmm0
	vcvttps2qqs xmm3, xmm6
	vcvttps2qqs ymm6, xmm3
	vcvttps2qqs ymm2, xmm6
	vcvttps2qqs zmm3, ymm7
	vcvttps2qqs zmm2, ymm2
	vcvttps2qqs xmm9, xmm9
	vcvttps2qqs ymm8, xmm12
	vcvttps2qqs zmm10, ymm14
	vcvttps2qqs xmm22, xmm18
	vcvttps2qqs ymm23, xmm16
	vcvttps2qqs zmm24, ymm29
	vcvttps2qqs xmm4{k4}, xmm1
	vcvttps2qqs ymm1{k4}, xmm0
	vcvttps2qqs zmm1{k1}, ymm4
	vcvttps2qqs xmm3{k1}{z}, xmm2
	vcvttps2qqs ymm3{k4}{z}, oword [0xe6c]
	vcvttps2qqs zmm5{k5}{z}, ymm5
	vcvttps2qqs zmm4, ymm0, {sae}
	vcvttps2qqs xmm2, qword [eax+1]
	vcvttps2qqs xmm7, qword [eax+64]
	vcvttps2qqs ymm1, oword [eax+1]
	vcvttps2qqs ymm0, oword [eax+64]
	vcvttps2qqs zmm6, yword [eax+1]
	vcvttps2qqs zmm5, yword [eax+64]
	vcvttps2qqs xmm7, [0x3d2]
	vcvttps2qqs ymm4, [0x538]
	vcvttps2qqs zmm6, [0xf39]
