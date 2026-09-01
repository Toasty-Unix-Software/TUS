default rel
	vcvtph2uqq xmm7, dword [0xc25]
	vcvtph2uqq xmm6, xmm4
	vcvtph2uqq ymm1, xmm4
	vcvtph2uqq ymm0, xmm1
	vcvtph2uqq zmm4, oword [0xf00]
	vcvtph2uqq zmm5, oword [0x469]
	vcvtph2uqq xmm9, xmm10
	vcvtph2uqq ymm9, xmm10
	vcvtph2uqq zmm13, xmm8
	vcvtph2uqq xmm18, xmm19
	vcvtph2uqq ymm16, xmm28
	vcvtph2uqq zmm26, xmm26
	vcvtph2uqq xmm4{k2}, xmm0
	vcvtph2uqq ymm2{k5}, qword [0x287]
	vcvtph2uqq zmm5{k6}, xmm0
	vcvtph2uqq xmm6{k3}{z}, xmm0
	vcvtph2uqq ymm7{k5}{z}, xmm0
	vcvtph2uqq zmm0{k4}{z}, oword [0x55a]
	vcvtph2uqq zmm2, xmm0, {rn-sae}
	vcvtph2uqq xmm6, dword [eax+1]
	vcvtph2uqq xmm2, dword [eax+64]
	vcvtph2uqq ymm0, qword [eax+1]
	vcvtph2uqq ymm2, qword [eax+64]
	vcvtph2uqq zmm6, oword [eax+1]
	vcvtph2uqq zmm6, oword [eax+64]
	vcvtph2uqq xmm4, [0xdf1]
	vcvtph2uqq ymm4, [0x7c3]
	vcvtph2uqq zmm3, [0x43c]
