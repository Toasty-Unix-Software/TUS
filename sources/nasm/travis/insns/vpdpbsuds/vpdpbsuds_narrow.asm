	vpdpbsuds xmm0, xmm5, xmm2
	vpdpbsuds xmm3, xmm0, oword [0xc06]
	vpdpbsuds ymm2, ymm3, yword [0x418]
	vpdpbsuds ymm0, ymm1, ymm4
	vpdpbsuds xmm6, xmm7, xmm4
	vpdpbsuds xmm5, xmm2, xmm5
	vpdpbsuds ymm5, ymm5, ymm0
	vpdpbsuds ymm1, ymm3, ymm7

%ifdef ERROR
	vpdpbsuds xmm15, xmm14, xmm15
	vpdpbsuds ymm11, ymm10, ymm13
	vpdpbsuds xmm13, xmm15, xmm8
	vpdpbsuds ymm15, ymm11, ymm15
	vpdpbsuds xmm22, xmm26, xmm24
	vpdpbsuds ymm21, ymm18, ymm25
	vpdpbsuds xmm22, xmm29, xmm27
	vpdpbsuds ymm23, ymm20, ymm22
%endif
