	vpdpbsud xmm7, xmm7, xmm3
	vpdpbsud xmm5, xmm7, xmm4
	vpdpbsud ymm6, ymm0, ymm2
	vpdpbsud ymm1, ymm7, ymm1
	vpdpbsud xmm5, xmm7, xmm2
	vpdpbsud xmm3, xmm7, xmm1
	vpdpbsud ymm2, ymm2, ymm5
	vpdpbsud ymm6, ymm7, ymm7

%ifdef ERROR
	vpdpbsud xmm15, xmm10, xmm12
	vpdpbsud ymm8, ymm15, ymm9
	vpdpbsud xmm10, xmm13, xmm11
	vpdpbsud ymm12, ymm14, ymm11
	vpdpbsud xmm19, xmm20, xmm17
	vpdpbsud ymm22, ymm30, ymm16
	vpdpbsud xmm31, xmm22, xmm24
	vpdpbsud ymm21, ymm22, ymm27
%endif
