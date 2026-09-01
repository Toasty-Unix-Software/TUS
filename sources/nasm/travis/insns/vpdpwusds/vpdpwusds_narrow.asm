	vpdpwusds xmm5, xmm5, oword [0x47c]
	vpdpwusds xmm5, xmm3, oword [0x283]
	vpdpwusds ymm4, ymm3, ymm1
	vpdpwusds ymm5, ymm6, ymm7
	vpdpwusds xmm1, xmm2, oword [0xd30]
	vpdpwusds xmm1, xmm5, xmm0
	vpdpwusds ymm2, ymm5, ymm3
	vpdpwusds ymm4, ymm1, ymm3

%ifdef ERROR
	vpdpwusds xmm11, xmm11, xmm12
	vpdpwusds ymm8, ymm10, ymm15
	vpdpwusds xmm13, xmm15, xmm15
	vpdpwusds ymm10, ymm11, ymm9
	vpdpwusds xmm18, xmm24, xmm30
	vpdpwusds ymm17, ymm22, ymm22
	vpdpwusds xmm20, xmm30, xmm23
	vpdpwusds ymm16, ymm21, ymm31
%endif
