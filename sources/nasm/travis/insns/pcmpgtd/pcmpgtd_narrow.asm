	pcmpgtd mm5, mm4
	pcmpgtd mm6, mm2
	pcmpgtd xmm5, xmm2
	pcmpgtd xmm6, xmm3

%ifdef ERROR
	pcmpgtd xmm8, xmm8
%endif
