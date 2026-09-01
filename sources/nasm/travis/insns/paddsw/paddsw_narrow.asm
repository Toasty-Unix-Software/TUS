	paddsw mm5, mm3
	paddsw mm5, mm5
	paddsw xmm5, oword [0xff4]
	paddsw xmm1, xmm4

%ifdef ERROR
	paddsw xmm8, xmm9
%endif
