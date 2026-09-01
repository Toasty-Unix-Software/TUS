	vphaddw xmm1, xmm0, xmm3
	vphaddw xmm1, xmm3
	vphaddw xmm4, xmm2, xmm6
	vphaddw ymm6, ymm4, ymm1
	vphaddw ymm6, ymm1
	vphaddw ymm0, ymm7, ymm3

%ifdef ERROR
	vphaddw xmm8, xmm15, xmm13
	vphaddw ymm13, ymm12, ymm10
%endif
