	vfnmsub213pd xmm2, xmm1, oword [0x107]
	vfnmsub213pd xmm1, xmm2, xmm3
	vfnmsub213pd ymm3, ymm3, ymm6
	vfnmsub213pd ymm3, ymm6, yword [0x3fb]
	vfnmsub213pd xmm6, xmm5, oword [0xe9a]
	vfnmsub213pd xmm0, xmm4, xmm6
	vfnmsub213pd ymm1, ymm5, ymm1
	vfnmsub213pd ymm2, ymm0, ymm0

%ifdef ERROR
	vfnmsub213pd xmm11, xmm15, xmm15
	vfnmsub213pd ymm11, ymm13, ymm11
	vfnmsub213pd xmm9, xmm12, xmm13
	vfnmsub213pd ymm10, ymm14, ymm11
	vfnmsub213pd xmm27, xmm21, xmm20
	vfnmsub213pd ymm27, ymm18, ymm25
	vfnmsub213pd xmm20, xmm21, xmm31
	vfnmsub213pd ymm18, ymm30, ymm25
%endif
