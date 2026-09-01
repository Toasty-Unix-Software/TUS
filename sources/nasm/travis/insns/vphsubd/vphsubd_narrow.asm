	vphsubd xmm6, xmm3, xmm6
	vphsubd xmm6, xmm6
	vphsubd xmm1, xmm6, xmm6
	vphsubd ymm5, ymm6, yword [0xf15]
	vphsubd ymm5, yword [0xf15]
	vphsubd ymm3, ymm2, ymm6

%ifdef ERROR
	vphsubd xmm12, xmm13, xmm12
	vphsubd ymm8, ymm12, ymm12
%endif
