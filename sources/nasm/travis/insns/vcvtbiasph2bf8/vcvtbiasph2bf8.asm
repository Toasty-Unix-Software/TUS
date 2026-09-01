default rel
	vcvtbiasph2bf8 xmm7, xmm4, oword [0x450]
	vcvtbiasph2bf8 xmm0, xmm1, oword [0x3bb]
	vcvtbiasph2bf8 xmm6, ymm2, ymm7
	vcvtbiasph2bf8 xmm4, ymm1, ymm7
	vcvtbiasph2bf8 ymm5, zmm0, zmm0
	vcvtbiasph2bf8 ymm4, zmm6, zmm4
	vcvtbiasph2bf8 xmm13, xmm15, xmm10
	vcvtbiasph2bf8 xmm15, ymm14, ymm9
	vcvtbiasph2bf8 ymm9, zmm14, zmm11
	vcvtbiasph2bf8 xmm25, xmm16, xmm28
	vcvtbiasph2bf8 xmm18, ymm19, ymm17
	vcvtbiasph2bf8 ymm24, zmm16, zmm23
	vcvtbiasph2bf8 xmm4{k3}, xmm1, xmm2
	vcvtbiasph2bf8 xmm2{k6}, ymm1, ymm1
	vcvtbiasph2bf8 ymm6{k7}, zmm0, zmm1
	vcvtbiasph2bf8 xmm6{k7}{z}, xmm6, oword [0xa67]
	vcvtbiasph2bf8 xmm2{k5}{z}, ymm3, yword [0x59d]
	vcvtbiasph2bf8 ymm4{k4}{z}, zmm1, zword [0xa67]
	vcvtbiasph2bf8 xmm7, xmm0, oword [eax+1]
	vcvtbiasph2bf8 xmm0, xmm1, oword [eax+64]
	vcvtbiasph2bf8 xmm4, ymm7, yword [eax+1]
	vcvtbiasph2bf8 xmm2, ymm2, yword [eax+64]
	vcvtbiasph2bf8 ymm7, zmm4, zword [eax+1]
	vcvtbiasph2bf8 ymm3, zmm0, zword [eax+64]
	vcvtbiasph2bf8 xmm0, xmm3, [0xe2a]
	vcvtbiasph2bf8 xmm1, ymm2, [0x47e]
	vcvtbiasph2bf8 ymm5, zmm6, [0x7d9]
