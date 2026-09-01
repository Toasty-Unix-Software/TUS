	vcmptrue_ussd xmm2, xmm1, qword [0x65e]
	vcmptrue_ussd xmm2, qword [0x65e]
	vcmptrue_ussd xmm3, xmm7, xmm5
	vcmptrue_ussd k1, xmm7, xmm3
	vcmptrue_ussd k2, xmm7, qword [0x3c4]

%ifdef ERROR
	vcmptrue_ussd xmm14, xmm9, xmm14
	vcmptrue_ussd k1, xmm12, xmm10
%endif
