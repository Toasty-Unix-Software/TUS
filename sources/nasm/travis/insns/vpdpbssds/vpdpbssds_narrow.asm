	vpdpbssds xmm4, xmm3, oword [0xb69]
	vpdpbssds xmm7, xmm7, xmm0
	vpdpbssds ymm1, ymm6, ymm3
	vpdpbssds ymm5, ymm1, ymm5
	vpdpbssds xmm6, xmm3, oword [0xd7c]
	vpdpbssds xmm4, xmm6, xmm5
	vpdpbssds ymm7, ymm4, yword [0x6d7]
	vpdpbssds ymm3, ymm4, yword [0xaba]

%ifdef ERROR
	vpdpbssds xmm8, xmm12, xmm8
	vpdpbssds ymm15, ymm9, ymm9
	vpdpbssds xmm8, xmm9, xmm10
	vpdpbssds ymm10, ymm8, ymm12
	vpdpbssds xmm22, xmm22, xmm30
	vpdpbssds ymm17, ymm21, ymm27
	vpdpbssds xmm31, xmm20, xmm25
	vpdpbssds ymm21, ymm27, ymm22
%endif
