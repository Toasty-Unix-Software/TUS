	vpmadcsswd xmm5, xmm5, xmm1, xmm0
	vpmadcsswd xmm5, xmm1, xmm0
	vpmadcsswd xmm6, xmm5, xmm3, xmm0

%ifdef ERROR
	vpmadcsswd xmm14, xmm8, xmm14, xmm10
%endif
