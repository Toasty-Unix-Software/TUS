	por mm0, qword [0xa0a]
	por mm3, mm2
	por xmm5, xmm1
	por xmm2, oword [0x161]

%ifdef ERROR
	por xmm14, xmm9
%endif
