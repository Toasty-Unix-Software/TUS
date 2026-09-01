	pxor mm7, qword [0xb3e]
	pxor mm1, mm7
	pxor xmm2, xmm4
	pxor xmm0, xmm6

%ifdef ERROR
	pxor xmm14, xmm13
%endif
