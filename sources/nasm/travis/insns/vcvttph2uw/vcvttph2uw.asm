default rel
	vcvttph2uw xmm1, oword [0x36d]
	vcvttph2uw xmm0, xmm7
	vcvttph2uw ymm5, ymm2
	vcvttph2uw ymm3, yword [0x1d9]
	vcvttph2uw zmm2, zmm0
	vcvttph2uw zmm6, zmm1
	vcvttph2uw xmm8, xmm9
	vcvttph2uw ymm14, ymm10
	vcvttph2uw zmm15, zmm9
	vcvttph2uw xmm20, xmm31
	vcvttph2uw ymm23, ymm21
	vcvttph2uw zmm22, zmm26
	vcvttph2uw xmm0{k3}, xmm2
	vcvttph2uw ymm6{k2}, ymm6
	vcvttph2uw zmm4{k6}, zmm4
	vcvttph2uw xmm3{k1}{z}, xmm3
	vcvttph2uw ymm1{k5}{z}, ymm0
	vcvttph2uw zmm7{k1}{z}, zmm1
	vcvttph2uw zmm4, zmm4, {sae}
	vcvttph2uw xmm6, oword [eax+1]
	vcvttph2uw xmm4, oword [eax+64]
	vcvttph2uw ymm1, yword [eax+1]
	vcvttph2uw ymm2, yword [eax+64]
	vcvttph2uw zmm4, zword [eax+1]
	vcvttph2uw zmm4, zword [eax+64]
	vcvttph2uw xmm7, [0xd0e]
	vcvttph2uw ymm7, [0xa01]
	vcvttph2uw zmm6, [0x8c3]
