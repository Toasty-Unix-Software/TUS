	vcmpunord_qsd xmm0, xmm1, xmm1
	vcmpunord_qsd xmm0, xmm1
	vcmpunord_qsd xmm1, xmm4, xmm2
	vcmpunord_qsd k1, xmm2, xmm4
	vcmpunord_qsd k5, xmm7, qword [0x9ce]

%ifdef ERROR
	vcmpunord_qsd xmm9, xmm11, xmm8
	vcmpunord_qsd k7, xmm15, xmm8
%endif
