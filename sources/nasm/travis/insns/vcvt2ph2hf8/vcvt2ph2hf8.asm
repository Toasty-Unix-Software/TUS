default rel
	vcvt2ph2hf8 xmm7, xmm6, xmm1
	vcvt2ph2hf8 xmm1, xmm4, xmm5
	vcvt2ph2hf8 ymm1, ymm5, ymm5
	vcvt2ph2hf8 ymm7, ymm2, ymm7
	vcvt2ph2hf8 zmm2, zmm7, zword [0xc7a]
	vcvt2ph2hf8 zmm1, zmm6, zmm1
	vcvt2ph2hf8 xmm10, xmm15, xmm12
	vcvt2ph2hf8 ymm15, ymm15, ymm11
	vcvt2ph2hf8 zmm12, zmm14, zmm13
	vcvt2ph2hf8 xmm24, xmm28, xmm23
	vcvt2ph2hf8 ymm28, ymm18, ymm31
	vcvt2ph2hf8 zmm16, zmm22, zmm18
	vcvt2ph2hf8 xmm5{k5}, xmm3, xmm1
	vcvt2ph2hf8 ymm2{k4}, ymm4, ymm0
	vcvt2ph2hf8 zmm5{k6}, zmm0, zword [0xded]
	vcvt2ph2hf8 xmm1{k6}{z}, xmm2, xmm1
	vcvt2ph2hf8 ymm2{k6}{z}, ymm6, ymm0
	vcvt2ph2hf8 zmm4{k1}{z}, zmm0, zmm1
	vcvt2ph2hf8 xmm0, xmm5, oword [eax+1]
	vcvt2ph2hf8 xmm5, xmm3, oword [eax+64]
	vcvt2ph2hf8 ymm1, ymm0, yword [eax+1]
	vcvt2ph2hf8 ymm5, ymm0, yword [eax+64]
	vcvt2ph2hf8 zmm1, zmm0, zword [eax+1]
	vcvt2ph2hf8 zmm2, zmm0, zword [eax+64]
	vcvt2ph2hf8 xmm0, xmm6, [0xf6e]
	vcvt2ph2hf8 ymm1, ymm6, [0x2ef]
	vcvt2ph2hf8 zmm2, zmm3, [0xaa7]
