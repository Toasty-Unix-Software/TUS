default rel
	vcvttph2udq xmm7, qword [0x96e]
	vcvttph2udq xmm4, xmm3
	vcvttph2udq ymm0, xmm2
	vcvttph2udq ymm3, oword [0x858]
	vcvttph2udq zmm6, ymm3
	vcvttph2udq zmm7, yword [0xc25]
	vcvttph2udq xmm15, xmm15
	vcvttph2udq ymm14, xmm12
	vcvttph2udq zmm11, ymm10
	vcvttph2udq xmm29, xmm17
	vcvttph2udq ymm24, xmm26
	vcvttph2udq zmm27, ymm22
	vcvttph2udq xmm6{k6}, xmm3
	vcvttph2udq ymm5{k3}, xmm7
	vcvttph2udq zmm7{k7}, yword [0xb6f]
	vcvttph2udq xmm2{k7}{z}, xmm2
	vcvttph2udq ymm5{k4}{z}, xmm2
	vcvttph2udq zmm6{k4}{z}, ymm4
	vcvttph2udq zmm1, ymm2, {sae}
	vcvttph2udq xmm7, qword [eax+1]
	vcvttph2udq xmm0, qword [eax+64]
	vcvttph2udq ymm0, oword [eax+1]
	vcvttph2udq ymm1, oword [eax+64]
	vcvttph2udq zmm4, yword [eax+1]
	vcvttph2udq zmm0, yword [eax+64]
	vcvttph2udq xmm0, [0xb79]
	vcvttph2udq ymm6, [0x7dd]
	vcvttph2udq zmm0, [0xf14]
