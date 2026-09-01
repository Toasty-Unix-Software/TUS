	unpckhpd xmm3, xmm6
	unpckhpd xmm7, xmm5

%ifdef ERROR
	unpckhpd xmm15, xmm10
%endif
