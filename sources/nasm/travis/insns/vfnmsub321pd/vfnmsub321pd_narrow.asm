	vfnmsub321pd xmm7, xmm5, oword [0x55e]
	vfnmsub321pd xmm2, xmm4, xmm2
	vfnmsub321pd ymm0, ymm3, ymm0
	vfnmsub321pd ymm5, ymm4, ymm5

%ifdef ERROR
	vfnmsub321pd xmm12, xmm12, xmm9
	vfnmsub321pd ymm8, ymm13, ymm10
%endif
