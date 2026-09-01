	vpdpwuuds xmm1, xmm6, oword [0xb5d]
	vpdpwuuds xmm5, xmm3, xmm7
	vpdpwuuds ymm1, ymm0, ymm7
	vpdpwuuds ymm4, ymm3, ymm6
	vpdpwuuds xmm0, xmm3, xmm6
	vpdpwuuds xmm1, xmm7, xmm4
	vpdpwuuds ymm7, ymm7, yword [0xee9]
	vpdpwuuds ymm4, ymm5, ymm1

%ifdef ERROR
	vpdpwuuds xmm11, xmm15, xmm15
	vpdpwuuds ymm11, ymm12, ymm12
	vpdpwuuds xmm13, xmm13, xmm12
	vpdpwuuds ymm8, ymm14, ymm9
	vpdpwuuds xmm30, xmm22, xmm28
	vpdpwuuds ymm17, ymm17, ymm24
	vpdpwuuds xmm16, xmm26, xmm21
	vpdpwuuds ymm29, ymm17, ymm24
%endif
