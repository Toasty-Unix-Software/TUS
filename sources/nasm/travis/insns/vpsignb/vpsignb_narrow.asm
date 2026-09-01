	vpsignb xmm0, xmm2, oword [0xb8a]
	vpsignb xmm0, oword [0xb8a]
	vpsignb xmm7, xmm4, xmm0
	vpsignb ymm2, ymm7, ymm7
	vpsignb ymm2, ymm7
	vpsignb ymm1, ymm6, ymm4

%ifdef ERROR
	vpsignb xmm9, xmm11, xmm8
	vpsignb ymm11, ymm9, ymm12
%endif
