	vpsignd xmm2, xmm3, xmm0
	vpsignd xmm2, xmm0
	vpsignd xmm0, xmm4, xmm3
	vpsignd ymm3, ymm0, ymm2
	vpsignd ymm3, ymm2
	vpsignd ymm2, ymm5, ymm6

%ifdef ERROR
	vpsignd xmm13, xmm13, xmm15
	vpsignd ymm14, ymm9, ymm15
%endif
