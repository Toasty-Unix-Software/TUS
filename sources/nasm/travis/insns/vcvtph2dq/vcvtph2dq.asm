default rel
	vcvtph2dq xmm0, qword [0x276]
	vcvtph2dq xmm4, xmm4
	vcvtph2dq ymm5, oword [0xa32]
	vcvtph2dq ymm1, xmm6
	vcvtph2dq zmm6, ymm4
	vcvtph2dq zmm3, ymm1
	vcvtph2dq xmm8, xmm14
	vcvtph2dq ymm14, xmm14
	vcvtph2dq zmm10, ymm15
	vcvtph2dq xmm23, xmm31
	vcvtph2dq ymm19, xmm22
	vcvtph2dq zmm17, ymm27
	vcvtph2dq xmm5{k5}, xmm1
	vcvtph2dq ymm3{k7}, oword [0x7ce]
	vcvtph2dq zmm7{k1}, ymm1
	vcvtph2dq xmm4{k7}{z}, xmm6
	vcvtph2dq ymm4{k1}{z}, oword [0x402]
	vcvtph2dq zmm3{k5}{z}, ymm1
	vcvtph2dq zmm6, ymm1, {rz-sae}
	vcvtph2dq xmm3, qword [eax+1]
	vcvtph2dq xmm6, qword [eax+64]
	vcvtph2dq ymm7, oword [eax+1]
	vcvtph2dq ymm3, oword [eax+64]
	vcvtph2dq zmm5, yword [eax+1]
	vcvtph2dq zmm7, yword [eax+64]
	vcvtph2dq xmm6, [0x678]
	vcvtph2dq ymm6, [0x436]
	vcvtph2dq zmm4, [0xa8d]
