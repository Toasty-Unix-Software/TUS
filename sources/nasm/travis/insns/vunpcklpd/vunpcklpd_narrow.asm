	vunpcklpd xmm7, xmm1, xmm0
	vunpcklpd xmm7, xmm0
	vunpcklpd xmm7, xmm3, xmm4
	vunpcklpd ymm0, ymm3, yword [0x8d5]
	vunpcklpd ymm0, yword [0x8d5]
	vunpcklpd ymm2, ymm2, ymm2
	vunpcklpd xmm4, xmm2, oword [0xd2e]
	vunpcklpd xmm4, oword [0xd2e]
	vunpcklpd xmm4, xmm7, oword [0x639]
	vunpcklpd ymm4, ymm3, ymm0
	vunpcklpd ymm4, ymm0
	vunpcklpd ymm7, ymm7, ymm5

%ifdef ERROR
	vunpcklpd xmm9, xmm8, xmm15
	vunpcklpd ymm15, ymm8, ymm14
	vunpcklpd xmm13, xmm11, xmm13
	vunpcklpd ymm9, ymm14, ymm11
	vunpcklpd xmm19, xmm17, xmm23
	vunpcklpd ymm17, ymm16, ymm28
	vunpcklpd xmm20, xmm23, xmm18
	vunpcklpd ymm20, ymm20, ymm30
%endif
