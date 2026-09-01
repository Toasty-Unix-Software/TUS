	paddsb mm3, mm2
	paddsb mm7, mm5
	paddsb xmm0, xmm4
	paddsb xmm5, oword [0x4ae]

%ifdef ERROR
	paddsb xmm11, xmm14
%endif
