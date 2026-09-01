default rel
	vcvtph2iubs xmm3, oword [0xe53]
	vcvtph2iubs xmm7, xmm7
	vcvtph2iubs ymm1, ymm2
	vcvtph2iubs ymm7, yword [0xa08]
	vcvtph2iubs zmm3, zword [0xdf3]
	vcvtph2iubs zmm4, zword [0x83a]
	vcvtph2iubs xmm10, xmm15
	vcvtph2iubs ymm15, ymm8
	vcvtph2iubs zmm11, zmm12
	vcvtph2iubs xmm27, xmm30
	vcvtph2iubs ymm25, ymm20
	vcvtph2iubs zmm27, zmm25
	vcvtph2iubs xmm3{k2}, oword [0xb22]
	vcvtph2iubs ymm4{k2}, ymm6
	vcvtph2iubs zmm1{k7}, zmm2
	vcvtph2iubs xmm3{k4}{z}, xmm3
	vcvtph2iubs ymm5{k5}{z}, ymm6
	vcvtph2iubs zmm7{k2}{z}, zmm6
	vcvtph2iubs zmm5, zmm7, {ru-sae}
	vcvtph2iubs xmm2, oword [eax+1]
	vcvtph2iubs xmm6, oword [eax+64]
	vcvtph2iubs ymm5, yword [eax+1]
	vcvtph2iubs ymm5, yword [eax+64]
	vcvtph2iubs zmm3, zword [eax+1]
	vcvtph2iubs zmm3, zword [eax+64]
	vcvtph2iubs xmm3, [0x3e5]
	vcvtph2iubs ymm4, [0xf7c]
	vcvtph2iubs zmm0, [0x89a]
