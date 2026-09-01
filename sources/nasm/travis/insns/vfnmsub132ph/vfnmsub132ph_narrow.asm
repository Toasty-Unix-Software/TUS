	vfnmsub132ph xmm5, xmm3, xmm7
	vfnmsub132ph xmm5, xmm7
	vfnmsub132ph xmm1, xmm6, xmm3
	vfnmsub132ph ymm0, ymm2, ymm2
	vfnmsub132ph ymm0, ymm2
	vfnmsub132ph ymm5, ymm7, ymm2
	vfnmsub132ph zmm2, zmm4, zword [0x89f]
	vfnmsub132ph zmm2, zword [0x89f]
	vfnmsub132ph zmm4, zmm7, zword [0x150]

%ifdef ERROR
	vfnmsub132ph xmm15, xmm9, xmm13
	vfnmsub132ph ymm11, ymm15, ymm10
	vfnmsub132ph zmm11, zmm10, zmm15
	vfnmsub132ph xmm19, xmm20, xmm25
	vfnmsub132ph ymm19, ymm31, ymm17
	vfnmsub132ph zmm18, zmm27, zmm27
%endif
