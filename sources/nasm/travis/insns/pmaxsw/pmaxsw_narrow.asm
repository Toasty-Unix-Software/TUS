	pmaxsw mm4, mm0
	pmaxsw mm1, mm6
	pmaxsw xmm3, xmm5
	pmaxsw xmm2, xmm1

%ifdef ERROR
	pmaxsw xmm10, xmm10
%endif
