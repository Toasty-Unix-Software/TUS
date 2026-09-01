	vfmadd312sd xmm4, xmm4, xmm0
	vfmadd312sd xmm2, xmm7, qword [0x817]

%ifdef ERROR
	vfmadd312sd xmm10, xmm13, xmm8
%endif
