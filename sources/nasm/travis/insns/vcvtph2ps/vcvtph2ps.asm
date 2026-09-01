default rel
	vcvtph2ps ymm1, xmm3
	vcvtph2ps ymm5, xmm7
	vcvtph2ps xmm0, qword [0x1f6]
	vcvtph2ps xmm1, qword [0xe87]
	vcvtph2ps xmm3, xmm2
	vcvtph2ps xmm5, xmm6
	vcvtph2ps ymm3, xmm6
	vcvtph2ps ymm7, xmm7
	vcvtph2ps ymm11, xmm9
	vcvtph2ps xmm10, xmm14
	vcvtph2ps xmm8, xmm8
	vcvtph2ps ymm10, xmm10
	vcvtph2ps ymm30, xmm20
	vcvtph2ps xmm30, xmm17
	vcvtph2ps xmm22, xmm24
	vcvtph2ps ymm20, xmm22
	vcvtph2ps xmm4{k3}, qword [0x602]
	vcvtph2ps ymm4{k4}, xmm4
	vcvtph2ps zmm7{k2}, ymm1
	vcvtph2ps xmm6{k2}{z}, xmm7
	vcvtph2ps ymm2{k7}{z}, xmm0
	vcvtph2ps zmm4{k2}{z}, ymm5
	vcvtph2ps zmm6, ymm6, {sae}
	vcvtph2ps ymm0, oword [eax+1]
	vcvtph2ps ymm0, oword [eax+64]
	vcvtph2ps xmm2, qword [eax+1]
	vcvtph2ps xmm0, qword [eax+64]
	vcvtph2ps xmm5, qword [eax+1]
	vcvtph2ps xmm7, qword [eax+64]
	vcvtph2ps ymm1, oword [eax+1]
	vcvtph2ps ymm5, oword [eax+64]
	vcvtph2ps zmm4, yword [eax+1]
	vcvtph2ps zmm2, yword [eax+64]
	vcvtph2ps ymm5, [0x7f6]
	vcvtph2ps xmm3, [0xda2]
	vcvtph2ps xmm5, [0x1a7]
	vcvtph2ps ymm4, [0x429]
	vcvtph2ps zmm3, [0xc1b]
