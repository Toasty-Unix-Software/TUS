	vpsignw xmm6, xmm6, xmm2
	vpsignw xmm6, xmm2
	vpsignw xmm4, xmm7, xmm1
	vpsignw ymm7, ymm4, yword [0x3c3]
	vpsignw ymm7, yword [0x3c3]
	vpsignw ymm5, ymm5, ymm4

%ifdef ERROR
	vpsignw xmm10, xmm10, xmm13
	vpsignw ymm10, ymm13, ymm15
%endif
