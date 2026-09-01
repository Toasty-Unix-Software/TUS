	vphsubsw xmm6, xmm1, oword [0x452]
	vphsubsw xmm6, oword [0x452]
	vphsubsw xmm6, xmm0, xmm5
	vphsubsw ymm3, ymm1, ymm6
	vphsubsw ymm3, ymm6
	vphsubsw ymm2, ymm2, ymm6

%ifdef ERROR
	vphsubsw xmm10, xmm12, xmm14
	vphsubsw ymm8, ymm15, ymm10
%endif
