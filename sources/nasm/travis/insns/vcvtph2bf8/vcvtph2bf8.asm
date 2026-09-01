default rel
	vcvtph2bf8 xmm5, oword [0x6fc]
	vcvtph2bf8 xmm2, oword [0x4f5]
	vcvtph2bf8 xmm4, yword [0xc9e]
	vcvtph2bf8 xmm2, yword [0xf77]
	vcvtph2bf8 ymm0, zmm3
	vcvtph2bf8 ymm1, zmm0
	vcvtph2bf8 xmm8, xmm12
	vcvtph2bf8 xmm13, ymm12
	vcvtph2bf8 ymm15, zmm14
	vcvtph2bf8 xmm27, xmm17
	vcvtph2bf8 xmm31, ymm20
	vcvtph2bf8 ymm29, zmm26
	vcvtph2bf8 xmm3{k2}, xmm2
	vcvtph2bf8 xmm2{k1}, ymm7
	vcvtph2bf8 ymm5{k2}, zmm4
	vcvtph2bf8 xmm4{k7}{z}, oword [0x5dc]
	vcvtph2bf8 xmm0{k1}{z}, ymm5
	vcvtph2bf8 ymm0{k4}{z}, zmm0
	vcvtph2bf8 xmm5, oword [eax+1]
	vcvtph2bf8 xmm3, oword [eax+64]
	vcvtph2bf8 xmm7, yword [eax+1]
	vcvtph2bf8 xmm5, yword [eax+64]
	vcvtph2bf8 ymm6, zword [eax+1]
	vcvtph2bf8 ymm6, zword [eax+64]
	vcvtph2bf8 xmm6, [0x500]
	vcvtph2bf8 xmm2, [0x1d2]
	vcvtph2bf8 ymm4, [0xfcf]
