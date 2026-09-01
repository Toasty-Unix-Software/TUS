default rel
	vcvt2ph2bf8 xmm1, xmm1, xmm4
	vcvt2ph2bf8 xmm5, xmm2, xmm6
	vcvt2ph2bf8 ymm7, ymm1, yword [0x14f]
	vcvt2ph2bf8 ymm2, ymm5, ymm6
	vcvt2ph2bf8 zmm7, zmm0, zmm2
	vcvt2ph2bf8 zmm6, zmm3, zmm2
	vcvt2ph2bf8 xmm10, xmm8, xmm12
	vcvt2ph2bf8 ymm14, ymm15, ymm9
	vcvt2ph2bf8 zmm11, zmm13, zmm9
	vcvt2ph2bf8 xmm23, xmm27, xmm17
	vcvt2ph2bf8 ymm16, ymm29, ymm16
	vcvt2ph2bf8 zmm30, zmm18, zmm18
	vcvt2ph2bf8 xmm6{k7}, xmm2, oword [0x6f6]
	vcvt2ph2bf8 ymm4{k5}, ymm7, ymm5
	vcvt2ph2bf8 zmm5{k7}, zmm6, zword [0xb76]
	vcvt2ph2bf8 xmm7{k3}{z}, xmm2, oword [0xfb4]
	vcvt2ph2bf8 ymm6{k4}{z}, ymm3, ymm1
	vcvt2ph2bf8 zmm4{k7}{z}, zmm3, zmm3
	vcvt2ph2bf8 xmm0, xmm5, oword [eax+1]
	vcvt2ph2bf8 xmm6, xmm5, oword [eax+64]
	vcvt2ph2bf8 ymm6, ymm4, yword [eax+1]
	vcvt2ph2bf8 ymm4, ymm4, yword [eax+64]
	vcvt2ph2bf8 zmm2, zmm3, zword [eax+1]
	vcvt2ph2bf8 zmm0, zmm1, zword [eax+64]
	vcvt2ph2bf8 xmm3, xmm6, [0xad6]
	vcvt2ph2bf8 ymm0, ymm7, [0x754]
	vcvt2ph2bf8 zmm7, zmm7, [0x254]
