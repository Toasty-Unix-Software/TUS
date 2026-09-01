	vphaddsw xmm7, xmm2, xmm4
	vphaddsw xmm7, xmm4
	vphaddsw xmm7, xmm3, xmm6
	vphaddsw ymm5, ymm0, yword [0xb75]
	vphaddsw ymm5, yword [0xb75]
	vphaddsw ymm7, ymm5, ymm4

%ifdef ERROR
	vphaddsw xmm13, xmm12, xmm10
	vphaddsw ymm14, ymm13, ymm15
%endif
