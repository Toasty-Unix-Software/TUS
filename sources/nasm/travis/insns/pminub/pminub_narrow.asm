	pminub mm4, qword [0x25a]
	pminub mm0, mm3
	pminub xmm2, xmm2
	pminub xmm5, xmm3

%ifdef ERROR
	pminub xmm12, xmm14
%endif
