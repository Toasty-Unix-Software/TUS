	vphaddd xmm7, xmm2, xmm6
	vphaddd xmm7, xmm6
	vphaddd xmm5, xmm2, oword [0xe3a]
	vphaddd ymm1, ymm0, ymm6
	vphaddd ymm1, ymm6
	vphaddd ymm4, ymm7, yword [0x6f1]

%ifdef ERROR
	vphaddd xmm11, xmm14, xmm14
	vphaddd ymm8, ymm12, ymm10
%endif
