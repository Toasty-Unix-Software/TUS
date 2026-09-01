default rel
	vcvttps2uqq xmm6, xmm5
	vcvttps2uqq xmm4, xmm3
	vcvttps2uqq ymm6, xmm1
	vcvttps2uqq ymm3, xmm1
	vcvttps2uqq zmm2, ymm5
	vcvttps2uqq zmm6, ymm3
	vcvttps2uqq xmm9, xmm15
	vcvttps2uqq ymm15, xmm14
	vcvttps2uqq zmm8, ymm14
	vcvttps2uqq xmm31, xmm26
	vcvttps2uqq ymm19, xmm20
	vcvttps2uqq zmm16, ymm17
	vcvttps2uqq xmm2{k4}, xmm3
	vcvttps2uqq ymm4{k2}, xmm6
	vcvttps2uqq zmm7{k4}, ymm2
	vcvttps2uqq xmm3{k4}{z}, xmm0
	vcvttps2uqq ymm0{k4}{z}, xmm7
	vcvttps2uqq zmm5{k5}{z}, ymm0
	vcvttps2uqq zmm4, ymm5, {sae}
	vcvttps2uqq xmm3, qword [eax+1]
	vcvttps2uqq xmm7, qword [eax+64]
	vcvttps2uqq ymm2, oword [eax+1]
	vcvttps2uqq ymm6, oword [eax+64]
	vcvttps2uqq zmm4, yword [eax+1]
	vcvttps2uqq zmm2, yword [eax+64]
	vcvttps2uqq xmm0, [0x93b]
	vcvttps2uqq ymm0, [0xe4b]
	vcvttps2uqq zmm1, [0xb42]
