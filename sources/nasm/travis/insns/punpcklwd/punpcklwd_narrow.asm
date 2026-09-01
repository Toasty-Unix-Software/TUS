	punpcklwd mm0, mm7
	punpcklwd mm6, qword [0x35f]
	punpcklwd xmm0, xmm0
	punpcklwd xmm3, xmm1

%ifdef ERROR
	punpcklwd xmm12, xmm13
%endif
