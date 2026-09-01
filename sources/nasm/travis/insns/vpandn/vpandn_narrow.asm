	vpandn xmm6, xmm1, xmm6
	vpandn xmm6, xmm6
	vpandn xmm2, xmm0, oword [0xc6c]
	vpandn ymm6, ymm1, ymm6
	vpandn ymm6, ymm6
	vpandn ymm2, ymm3, yword [0xa9c]

%ifdef ERROR
	vpandn xmm9, xmm9, xmm14
	vpandn ymm10, ymm13, ymm13
%endif
