	vpmacssdql xmm6, xmm1, oword [0x9fb], xmm4
	vpmacssdql xmm6, oword [0x9fb], xmm4
	vpmacssdql xmm2, xmm7, oword [0x55d], xmm1

%ifdef ERROR
	vpmacssdql xmm11, xmm14, xmm12, xmm12
%endif
