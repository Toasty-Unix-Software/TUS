	vrcp28ss xmm1, xmm3, xmm5
	vrcp28ss xmm1, xmm5
	vrcp28ss xmm3, xmm5, xmm7

%ifdef ERROR
	vrcp28ss xmm14, xmm15, xmm10
	vrcp28ss xmm26, xmm17, xmm23
%endif
