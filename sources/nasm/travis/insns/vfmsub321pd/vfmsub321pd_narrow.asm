	vfmsub321pd xmm3, xmm2, xmm6
	vfmsub321pd xmm5, xmm5, oword [0xf19]
	vfmsub321pd ymm6, ymm3, ymm5
	vfmsub321pd ymm3, ymm5, yword [0x207]

%ifdef ERROR
	vfmsub321pd xmm11, xmm13, xmm13
	vfmsub321pd ymm12, ymm9, ymm12
%endif
