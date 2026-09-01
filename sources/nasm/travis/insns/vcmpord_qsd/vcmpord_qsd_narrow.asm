	vcmpord_qsd xmm0, xmm6, xmm7
	vcmpord_qsd xmm0, xmm7
	vcmpord_qsd xmm3, xmm0, xmm3
	vcmpord_qsd k2, xmm4, xmm2
	vcmpord_qsd k5, xmm5, qword [0x1a1]

%ifdef ERROR
	vcmpord_qsd xmm14, xmm12, xmm14
	vcmpord_qsd k1, xmm8, xmm15
%endif
