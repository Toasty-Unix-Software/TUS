default rel
	vcvtbiasph2hf8 xmm3, xmm1, xmm3
	vcvtbiasph2hf8 xmm2, xmm7, xmm5
	vcvtbiasph2hf8 xmm0, ymm1, ymm4
	vcvtbiasph2hf8 xmm4, ymm2, ymm0
	vcvtbiasph2hf8 ymm2, zmm3, zmm5
	vcvtbiasph2hf8 ymm4, zmm3, zmm4
	vcvtbiasph2hf8 xmm15, xmm14, xmm10
	vcvtbiasph2hf8 xmm11, ymm14, ymm8
	vcvtbiasph2hf8 ymm11, zmm12, zmm15
	vcvtbiasph2hf8 xmm18, xmm25, xmm24
	vcvtbiasph2hf8 xmm28, ymm25, ymm24
	vcvtbiasph2hf8 ymm16, zmm26, zmm17
	vcvtbiasph2hf8 xmm4{k5}, xmm5, xmm0
	vcvtbiasph2hf8 xmm6{k7}, ymm4, yword [0x2b5]
	vcvtbiasph2hf8 ymm1{k6}, zmm7, zword [0x541]
	vcvtbiasph2hf8 xmm7{k7}{z}, xmm1, xmm5
	vcvtbiasph2hf8 xmm2{k6}{z}, ymm3, ymm0
	vcvtbiasph2hf8 ymm0{k5}{z}, zmm6, zmm2
	vcvtbiasph2hf8 xmm7, xmm5, oword [eax+1]
	vcvtbiasph2hf8 xmm0, xmm0, oword [eax+64]
	vcvtbiasph2hf8 xmm0, ymm0, yword [eax+1]
	vcvtbiasph2hf8 xmm1, ymm2, yword [eax+64]
	vcvtbiasph2hf8 ymm6, zmm0, zword [eax+1]
	vcvtbiasph2hf8 ymm7, zmm2, zword [eax+64]
	vcvtbiasph2hf8 xmm6, xmm6, [0x82a]
	vcvtbiasph2hf8 xmm5, ymm3, [0xd11]
	vcvtbiasph2hf8 ymm5, zmm3, [0x482]
