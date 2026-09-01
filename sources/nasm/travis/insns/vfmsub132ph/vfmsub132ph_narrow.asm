	vfmsub132ph xmm5, xmm0, xmm3
	vfmsub132ph xmm5, xmm3
	vfmsub132ph xmm3, xmm5, xmm7
	vfmsub132ph ymm4, ymm5, ymm3
	vfmsub132ph ymm4, ymm3
	vfmsub132ph ymm1, ymm0, ymm6
	vfmsub132ph zmm3, zmm0, zmm0
	vfmsub132ph zmm3, zmm0
	vfmsub132ph zmm6, zmm1, zmm7

%ifdef ERROR
	vfmsub132ph xmm13, xmm9, xmm9
	vfmsub132ph ymm9, ymm10, ymm14
	vfmsub132ph zmm11, zmm13, zmm15
	vfmsub132ph xmm31, xmm27, xmm21
	vfmsub132ph ymm21, ymm18, ymm19
	vfmsub132ph zmm20, zmm19, zmm18
%endif
