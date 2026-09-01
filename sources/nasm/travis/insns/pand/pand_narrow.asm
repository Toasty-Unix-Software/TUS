	pand mm4, mm3
	pand mm5, qword [0x40b]
	pand xmm5, xmm2
	pand xmm2, xmm6

%ifdef ERROR
	pand xmm10, xmm15
%endif
