	vpand xmm5, xmm1, oword [0xe0b]
	vpand xmm5, oword [0xe0b]
	vpand xmm1, xmm0, oword [0xf38]
	vpand ymm5, ymm3, ymm4
	vpand ymm5, ymm4
	vpand ymm6, ymm2, ymm3

%ifdef ERROR
	vpand xmm10, xmm13, xmm12
	vpand ymm10, ymm13, ymm9
%endif
