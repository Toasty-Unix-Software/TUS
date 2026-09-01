default rel
	vcvtbf162iubs xmm4, xmm4
	vcvtbf162iubs xmm0, xmm4
	vcvtbf162iubs ymm7, ymm0
	vcvtbf162iubs ymm4, ymm2
	vcvtbf162iubs zmm3, zword [0x182]
	vcvtbf162iubs zmm7, zmm3
	vcvtbf162iubs xmm12, xmm11
	vcvtbf162iubs ymm11, ymm11
	vcvtbf162iubs zmm13, zmm12
	vcvtbf162iubs xmm19, xmm30
	vcvtbf162iubs ymm30, ymm24
	vcvtbf162iubs zmm23, zmm31
	vcvtbf162iubs xmm1{k2}, xmm0
	vcvtbf162iubs ymm5{k5}, yword [0xb05]
	vcvtbf162iubs zmm1{k6}, zmm3
	vcvtbf162iubs xmm1{k3}{z}, xmm3
	vcvtbf162iubs ymm4{k5}{z}, ymm3
	vcvtbf162iubs zmm7{k2}{z}, zmm0
	vcvtbf162iubs xmm0, oword [eax+1]
	vcvtbf162iubs xmm5, oword [eax+64]
	vcvtbf162iubs ymm7, yword [eax+1]
	vcvtbf162iubs ymm5, yword [eax+64]
	vcvtbf162iubs zmm6, zword [eax+1]
	vcvtbf162iubs zmm0, zword [eax+64]
	vcvtbf162iubs xmm7, [0x7da]
	vcvtbf162iubs ymm1, [0x83b]
	vcvtbf162iubs zmm5, [0x1f6]
