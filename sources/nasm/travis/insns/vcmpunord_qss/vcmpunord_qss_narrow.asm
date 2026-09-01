	vcmpunord_qss xmm4, xmm7, xmm0
	vcmpunord_qss xmm4, xmm0
	vcmpunord_qss xmm7, xmm7, xmm3
	vcmpunord_qss k3, xmm6, xmm5
	vcmpunord_qss k7, xmm4, xmm3

%ifdef ERROR
	vcmpunord_qss xmm8, xmm11, xmm14
	vcmpunord_qss k7, xmm13, xmm8
%endif
