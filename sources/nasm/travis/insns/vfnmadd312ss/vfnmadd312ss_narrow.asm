	vfnmadd312ss xmm7, xmm4, xmm3
	vfnmadd312ss xmm5, xmm0, xmm7

%ifdef ERROR
	vfnmadd312ss xmm15, xmm11, xmm14
%endif
