	vfmadd123sd xmm0, xmm4, qword [0xcd5]
	vfmadd123sd xmm2, xmm0, qword [0xd37]

%ifdef ERROR
	vfmadd123sd xmm13, xmm8, xmm11
%endif
