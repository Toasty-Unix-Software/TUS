	vpdpwssd xmm3, xmm5, oword [0xd33]
	vpdpwssd xmm0, xmm0, oword [0xa6b]
	vpdpwssd ymm2, ymm1, ymm7
	vpdpwssd ymm7, ymm2, yword [0xd0d]
	vpdpwssd xmm7, xmm7, xmm5
	vpdpwssd xmm7, xmm5
	vpdpwssd xmm0, xmm4, xmm4
	vpdpwssd ymm4, ymm6, yword [0x686]
	vpdpwssd ymm4, yword [0x686]
	vpdpwssd ymm0, ymm5, ymm4

%ifdef ERROR
	vpdpwssd xmm13, xmm11, xmm12
	vpdpwssd ymm10, ymm8, ymm13
	vpdpwssd xmm14, xmm9, xmm9
	vpdpwssd ymm13, ymm12, ymm11
	vpdpwssd xmm21, xmm22, xmm26
	vpdpwssd ymm30, ymm30, ymm28
	vpdpwssd xmm30, xmm19, xmm30
	vpdpwssd ymm27, ymm18, ymm23
%endif
