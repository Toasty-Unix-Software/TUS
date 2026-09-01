	vpdpbusds xmm2, xmm4, xmm1
	vpdpbusds xmm0, xmm6, xmm2
	vpdpbusds ymm5, ymm7, ymm4
	vpdpbusds ymm1, ymm5, ymm1
	vpdpbusds xmm5, xmm0, xmm3
	vpdpbusds xmm5, xmm3
	vpdpbusds xmm0, xmm0, oword [0xd7b]
	vpdpbusds ymm6, ymm6, yword [0xa0e]
	vpdpbusds ymm6, yword [0xa0e]
	vpdpbusds ymm7, ymm2, ymm0

%ifdef ERROR
	vpdpbusds xmm13, xmm15, xmm10
	vpdpbusds ymm9, ymm14, ymm11
	vpdpbusds xmm8, xmm9, xmm8
	vpdpbusds ymm15, ymm11, ymm10
	vpdpbusds xmm30, xmm20, xmm28
	vpdpbusds ymm24, ymm28, ymm30
	vpdpbusds xmm18, xmm18, xmm16
	vpdpbusds ymm24, ymm17, ymm18
%endif
