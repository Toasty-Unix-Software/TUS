	vpdpwssds xmm1, xmm0, oword [0xc27]
	vpdpwssds xmm0, xmm3, xmm1
	vpdpwssds ymm4, ymm4, ymm2
	vpdpwssds ymm3, ymm7, yword [0x49b]
	vpdpwssds xmm5, xmm6, xmm1
	vpdpwssds xmm5, xmm1
	vpdpwssds xmm3, xmm0, xmm1
	vpdpwssds ymm6, ymm5, ymm2
	vpdpwssds ymm6, ymm2
	vpdpwssds ymm1, ymm2, ymm6

%ifdef ERROR
	vpdpwssds xmm8, xmm12, xmm10
	vpdpwssds ymm11, ymm11, ymm11
	vpdpwssds xmm12, xmm9, xmm10
	vpdpwssds ymm8, ymm11, ymm12
	vpdpwssds xmm31, xmm22, xmm30
	vpdpwssds ymm18, ymm23, ymm28
	vpdpwssds xmm25, xmm18, xmm26
	vpdpwssds ymm30, ymm30, ymm31
%endif
