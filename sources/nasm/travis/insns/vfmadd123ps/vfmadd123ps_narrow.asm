	vfmadd123ps xmm2, xmm4, xmm3
	vfmadd123ps xmm1, xmm6, xmm2
	vfmadd123ps ymm5, ymm1, ymm5
	vfmadd123ps ymm4, ymm2, ymm2

%ifdef ERROR
	vfmadd123ps xmm15, xmm9, xmm9
	vfmadd123ps ymm15, ymm8, ymm12
%endif
