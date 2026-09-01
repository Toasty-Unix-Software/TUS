	vcmpord_qss xmm7, xmm2, xmm4
	vcmpord_qss xmm7, xmm4
	vcmpord_qss xmm7, xmm6, qword [0xd5c]
	vcmpord_qss k3, xmm5, xmm1
	vcmpord_qss k2, xmm1, dword [0x662]

%ifdef ERROR
	vcmpord_qss xmm13, xmm8, xmm9
	vcmpord_qss k1, xmm12, xmm10
%endif
