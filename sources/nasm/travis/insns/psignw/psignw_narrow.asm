	psignw mm1, mm3
	psignw mm4, mm0
	psignw xmm5, xmm7
	psignw xmm6, oword [0xbc4]

%ifdef ERROR
	psignw xmm9, xmm8
%endif
