	vunpckhpd xmm6, xmm1, xmm6
	vunpckhpd xmm6, xmm6
	vunpckhpd xmm7, xmm7, xmm5
	vunpckhpd ymm1, ymm4, yword [0x335]
	vunpckhpd ymm1, yword [0x335]
	vunpckhpd ymm2, ymm5, ymm1
	vunpckhpd xmm5, xmm2, xmm1
	vunpckhpd xmm5, xmm1
	vunpckhpd xmm5, xmm4, xmm1
	vunpckhpd ymm5, ymm4, ymm4
	vunpckhpd ymm5, ymm4
	vunpckhpd ymm6, ymm2, ymm3

%ifdef ERROR
	vunpckhpd xmm13, xmm15, xmm11
	vunpckhpd ymm8, ymm14, ymm15
	vunpckhpd xmm10, xmm12, xmm13
	vunpckhpd ymm10, ymm9, ymm11
	vunpckhpd xmm30, xmm24, xmm30
	vunpckhpd ymm27, ymm17, ymm20
	vunpckhpd xmm22, xmm18, xmm16
	vunpckhpd ymm30, ymm28, ymm30
%endif
