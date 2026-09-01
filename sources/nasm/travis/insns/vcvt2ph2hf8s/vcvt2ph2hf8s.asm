default rel
	vcvt2ph2hf8s xmm2, xmm4, xmm1
	vcvt2ph2hf8s xmm6, xmm4, xmm6
	vcvt2ph2hf8s ymm3, ymm1, ymm0
	vcvt2ph2hf8s ymm4, ymm2, ymm5
	vcvt2ph2hf8s zmm5, zmm3, zword [0x2f9]
	vcvt2ph2hf8s zmm6, zmm0, zmm3
	vcvt2ph2hf8s xmm8, xmm12, xmm14
	vcvt2ph2hf8s ymm9, ymm15, ymm11
	vcvt2ph2hf8s zmm8, zmm14, zmm13
	vcvt2ph2hf8s xmm24, xmm31, xmm29
	vcvt2ph2hf8s ymm27, ymm19, ymm25
	vcvt2ph2hf8s zmm18, zmm17, zmm24
	vcvt2ph2hf8s xmm3{k5}, xmm3, xmm1
	vcvt2ph2hf8s ymm0{k1}, ymm1, ymm0
	vcvt2ph2hf8s zmm7{k4}, zmm5, zmm1
	vcvt2ph2hf8s xmm7{k3}{z}, xmm3, xmm1
	vcvt2ph2hf8s ymm0{k6}{z}, ymm2, yword [0x808]
	vcvt2ph2hf8s zmm4{k3}{z}, zmm3, zmm5
	vcvt2ph2hf8s xmm0, xmm0, oword [eax+1]
	vcvt2ph2hf8s xmm0, xmm1, oword [eax+64]
	vcvt2ph2hf8s ymm0, ymm3, yword [eax+1]
	vcvt2ph2hf8s ymm1, ymm2, yword [eax+64]
	vcvt2ph2hf8s zmm1, zmm0, zword [eax+1]
	vcvt2ph2hf8s zmm3, zmm1, zword [eax+64]
	vcvt2ph2hf8s xmm5, xmm4, [0x3f0]
	vcvt2ph2hf8s ymm4, ymm7, [0x8dc]
	vcvt2ph2hf8s zmm1, zmm1, [0x511]
