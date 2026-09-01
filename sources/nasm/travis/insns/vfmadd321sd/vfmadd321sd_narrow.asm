	vfmadd321sd xmm2, xmm5, qword [0xab2]
	vfmadd321sd xmm1, xmm6, xmm0

%ifdef ERROR
	vfmadd321sd xmm13, xmm15, xmm15
%endif
