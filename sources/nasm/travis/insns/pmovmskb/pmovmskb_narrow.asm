	pmovmskb edi, mm5
	pmovmskb ebp, mm2
	pmovmskb edx, xmm1
	pmovmskb ebp, xmm5

%ifdef ERROR
	pmovmskb r9d, mm2
	pmovmskb r12d, xmm10
%endif
