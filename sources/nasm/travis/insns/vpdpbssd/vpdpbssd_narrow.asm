	vpdpbssd xmm5, xmm3, xmm0
	vpdpbssd xmm0, xmm4, xmm6
	vpdpbssd ymm4, ymm2, ymm7
	vpdpbssd ymm4, ymm1, ymm2
	vpdpbssd xmm7, xmm3, xmm1
	vpdpbssd xmm2, xmm7, xmm5
	vpdpbssd ymm6, ymm0, ymm5
	vpdpbssd ymm2, ymm4, yword [0xe57]

%ifdef ERROR
	vpdpbssd xmm12, xmm14, xmm15
	vpdpbssd ymm13, ymm8, ymm14
	vpdpbssd xmm11, xmm8, xmm9
	vpdpbssd ymm14, ymm11, ymm15
	vpdpbssd xmm23, xmm28, xmm23
	vpdpbssd ymm17, ymm24, ymm19
	vpdpbssd xmm20, xmm31, xmm22
	vpdpbssd ymm20, ymm23, ymm20
%endif
