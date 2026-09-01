default rel
	vcvtph2udq xmm6, xmm2
	vcvtph2udq xmm7, xmm0
	vcvtph2udq ymm3, xmm2
	vcvtph2udq ymm3, xmm6
	vcvtph2udq zmm4, ymm0
	vcvtph2udq zmm3, yword [0xb0c]
	vcvtph2udq xmm10, xmm9
	vcvtph2udq ymm9, xmm14
	vcvtph2udq zmm14, ymm10
	vcvtph2udq xmm21, xmm31
	vcvtph2udq ymm18, xmm30
	vcvtph2udq zmm22, ymm17
	vcvtph2udq xmm0{k3}, xmm3
	vcvtph2udq ymm2{k3}, xmm1
	vcvtph2udq zmm3{k2}, yword [0xdfd]
	vcvtph2udq xmm2{k1}{z}, xmm3
	vcvtph2udq ymm3{k5}{z}, oword [0xcec]
	vcvtph2udq zmm1{k7}{z}, yword [0x9f5]
	vcvtph2udq zmm0, ymm5, {ru-sae}
	vcvtph2udq xmm2, qword [eax+1]
	vcvtph2udq xmm0, qword [eax+64]
	vcvtph2udq ymm4, oword [eax+1]
	vcvtph2udq ymm3, oword [eax+64]
	vcvtph2udq zmm4, yword [eax+1]
	vcvtph2udq zmm7, yword [eax+64]
	vcvtph2udq xmm7, [0x221]
	vcvtph2udq ymm0, [0x22c]
	vcvtph2udq zmm1, [0x624]
