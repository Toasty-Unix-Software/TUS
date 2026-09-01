default rel
	vcvttph2dq xmm5, xmm4
	vcvttph2dq xmm4, xmm6
	vcvttph2dq ymm6, xmm7
	vcvttph2dq ymm0, xmm7
	vcvttph2dq zmm0, yword [0x3df]
	vcvttph2dq zmm4, ymm7
	vcvttph2dq xmm15, xmm14
	vcvttph2dq ymm13, xmm13
	vcvttph2dq zmm10, ymm9
	vcvttph2dq xmm18, xmm17
	vcvttph2dq ymm20, xmm16
	vcvttph2dq zmm21, ymm27
	vcvttph2dq xmm1{k6}, xmm1
	vcvttph2dq ymm0{k4}, xmm5
	vcvttph2dq zmm7{k1}, yword [0x56f]
	vcvttph2dq xmm7{k4}{z}, qword [0xee2]
	vcvttph2dq ymm7{k3}{z}, xmm5
	vcvttph2dq zmm7{k7}{z}, ymm3
	vcvttph2dq zmm2, ymm6, {sae}
	vcvttph2dq xmm2, qword [eax+1]
	vcvttph2dq xmm3, qword [eax+64]
	vcvttph2dq ymm4, oword [eax+1]
	vcvttph2dq ymm1, oword [eax+64]
	vcvttph2dq zmm0, yword [eax+1]
	vcvttph2dq zmm4, yword [eax+64]
	vcvttph2dq xmm6, [0xec1]
	vcvttph2dq ymm1, [0x673]
	vcvttph2dq zmm4, [0x1f7]
