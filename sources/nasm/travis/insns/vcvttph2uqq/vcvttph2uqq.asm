default rel
	vcvttph2uqq xmm6, xmm2
	vcvttph2uqq xmm5, dword [0x9ce]
	vcvttph2uqq ymm7, xmm2
	vcvttph2uqq ymm0, xmm5
	vcvttph2uqq zmm0, xmm1
	vcvttph2uqq zmm0, xmm1
	vcvttph2uqq xmm11, xmm12
	vcvttph2uqq ymm10, xmm13
	vcvttph2uqq zmm8, xmm15
	vcvttph2uqq xmm24, xmm30
	vcvttph2uqq ymm17, xmm22
	vcvttph2uqq zmm17, xmm22
	vcvttph2uqq xmm3{k1}, dword [0xf2c]
	vcvttph2uqq ymm6{k1}, xmm4
	vcvttph2uqq zmm6{k2}, oword [0x170]
	vcvttph2uqq xmm1{k1}{z}, xmm0
	vcvttph2uqq ymm7{k7}{z}, qword [0xdd4]
	vcvttph2uqq zmm5{k2}{z}, oword [0xbf2]
	vcvttph2uqq zmm4, xmm5, {sae}
	vcvttph2uqq xmm2, dword [eax+1]
	vcvttph2uqq xmm7, dword [eax+64]
	vcvttph2uqq ymm6, qword [eax+1]
	vcvttph2uqq ymm1, qword [eax+64]
	vcvttph2uqq zmm7, oword [eax+1]
	vcvttph2uqq zmm5, oword [eax+64]
	vcvttph2uqq xmm1, [0x340]
	vcvttph2uqq ymm5, [0xf6d]
	vcvttph2uqq zmm0, [0x753]
