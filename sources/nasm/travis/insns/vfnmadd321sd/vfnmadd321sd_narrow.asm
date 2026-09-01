	vfnmadd321sd xmm5, xmm7, xmm6
	vfnmadd321sd xmm2, xmm4, qword [0xef7]

%ifdef ERROR
	vfnmadd321sd xmm14, xmm13, xmm11
%endif
