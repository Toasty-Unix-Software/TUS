	vfmadd123ss xmm0, xmm5, xmm0
	vfmadd123ss xmm7, xmm2, xmm5

%ifdef ERROR
	vfmadd123ss xmm9, xmm9, xmm9
%endif
