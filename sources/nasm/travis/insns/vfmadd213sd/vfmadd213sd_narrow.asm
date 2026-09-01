	vfmadd213sd xmm6, xmm5, xmm7
	vfmadd213sd xmm4, xmm1, xmm0
	vfmadd213sd xmm7, xmm3, qword [0x12e]
	vfmadd213sd xmm4, xmm7, xmm4

%ifdef ERROR
	vfmadd213sd xmm9, xmm9, xmm14
	vfmadd213sd xmm11, xmm9, xmm13
	vfmadd213sd xmm30, xmm27, xmm16
	vfmadd213sd xmm17, xmm26, xmm27
%endif
