default rel
	vcvtph2hf8 xmm4, xmm0
	vcvtph2hf8 xmm6, xmm0
	vcvtph2hf8 xmm1, yword [0x82f]
	vcvtph2hf8 xmm6, ymm0
	vcvtph2hf8 ymm7, zword [0xe79]
	vcvtph2hf8 ymm7, zmm2
	vcvtph2hf8 xmm13, xmm10
	vcvtph2hf8 xmm10, ymm9
	vcvtph2hf8 ymm11, zmm14
	vcvtph2hf8 xmm29, xmm24
	vcvtph2hf8 xmm27, ymm19
	vcvtph2hf8 ymm21, zmm27
	vcvtph2hf8 xmm5{k7}, xmm4
	vcvtph2hf8 xmm6{k1}, yword [0xaf8]
	vcvtph2hf8 ymm0{k2}, zmm4
	vcvtph2hf8 xmm5{k4}{z}, oword [0x67b]
	vcvtph2hf8 xmm5{k1}{z}, ymm1
	vcvtph2hf8 ymm5{k1}{z}, zmm4
	vcvtph2hf8 xmm6, oword [eax+1]
	vcvtph2hf8 xmm6, oword [eax+64]
	vcvtph2hf8 xmm0, yword [eax+1]
	vcvtph2hf8 xmm1, yword [eax+64]
	vcvtph2hf8 ymm6, zword [eax+1]
	vcvtph2hf8 ymm4, zword [eax+64]
	vcvtph2hf8 xmm6, [0xa70]
	vcvtph2hf8 xmm2, [0x86c]
	vcvtph2hf8 ymm1, [0xca7]
