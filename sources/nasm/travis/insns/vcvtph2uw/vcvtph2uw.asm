default rel
	vcvtph2uw xmm7, xmm4
	vcvtph2uw xmm5, xmm4
	vcvtph2uw ymm0, ymm0
	vcvtph2uw ymm4, ymm3
	vcvtph2uw zmm4, zword [0xc35]
	vcvtph2uw zmm7, zmm4
	vcvtph2uw xmm14, xmm12
	vcvtph2uw ymm15, ymm10
	vcvtph2uw zmm10, zmm13
	vcvtph2uw xmm27, xmm18
	vcvtph2uw ymm16, ymm25
	vcvtph2uw zmm23, zmm20
	vcvtph2uw xmm0{k5}, xmm1
	vcvtph2uw ymm5{k7}, ymm4
	vcvtph2uw zmm6{k5}, zword [0x9b9]
	vcvtph2uw xmm6{k3}{z}, xmm2
	vcvtph2uw ymm0{k3}{z}, yword [0xabf]
	vcvtph2uw zmm7{k4}{z}, zword [0xc3b]
	vcvtph2uw zmm5, zmm2, {rd-sae}
	vcvtph2uw xmm5, oword [eax+1]
	vcvtph2uw xmm4, oword [eax+64]
	vcvtph2uw ymm4, yword [eax+1]
	vcvtph2uw ymm4, yword [eax+64]
	vcvtph2uw zmm6, zword [eax+1]
	vcvtph2uw zmm1, zword [eax+64]
	vcvtph2uw xmm3, [0x7c8]
	vcvtph2uw ymm4, [0x2bc]
	vcvtph2uw zmm5, [0xa44]
