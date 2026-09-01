	vcmpeq_ussd xmm2, xmm3, qword [0x9c1]
	vcmpeq_ussd xmm2, qword [0x9c1]
	vcmpeq_ussd xmm5, xmm5, xmm3
	vcmpeq_ussd k7, xmm1, xmm4
	vcmpeq_ussd k2, xmm6, qword [0x150]

%ifdef ERROR
	vcmpeq_ussd xmm15, xmm10, xmm9
	vcmpeq_ussd k4, xmm13, xmm15
%endif
