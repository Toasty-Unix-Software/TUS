	pandn mm3, qword [0xcd9]
	pandn mm5, mm7
	pandn xmm1, xmm7
	pandn xmm7, xmm0

%ifdef ERROR
	pandn xmm9, xmm11
%endif
