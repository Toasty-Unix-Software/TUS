	vcvt2ph2hf8 xmm7, xmm6, xmm1
	vcvt2ph2hf8 xmm1, xmm4, xmm5
	vcvt2ph2hf8 ymm1, ymm5, ymm5
	vcvt2ph2hf8 ymm7, ymm2, ymm7
	vcvt2ph2hf8 zmm2, zmm7, zword [0xc7a]
	vcvt2ph2hf8 zmm1, zmm6, zmm1

%ifdef ERROR
	vcvt2ph2hf8 xmm10, xmm15, xmm12
	vcvt2ph2hf8 ymm15, ymm15, ymm11
	vcvt2ph2hf8 zmm12, zmm14, zmm13
	vcvt2ph2hf8 xmm24, xmm28, xmm23
	vcvt2ph2hf8 ymm28, ymm18, ymm31
	vcvt2ph2hf8 zmm16, zmm22, zmm18
%endif
