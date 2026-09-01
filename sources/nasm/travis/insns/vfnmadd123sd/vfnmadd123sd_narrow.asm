	vfnmadd123sd xmm3, xmm3, xmm2
	vfnmadd123sd xmm1, xmm6, xmm1

%ifdef ERROR
	vfnmadd123sd xmm12, xmm11, xmm13
%endif
