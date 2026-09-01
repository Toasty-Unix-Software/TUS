	vcmpneq_ussd xmm7, xmm5, xmm6
	vcmpneq_ussd xmm7, xmm6
	vcmpneq_ussd xmm1, xmm1, xmm0
	vcmpneq_ussd k2, xmm5, qword [0x9f5]
	vcmpneq_ussd k6, xmm6, xmm0

%ifdef ERROR
	vcmpneq_ussd xmm14, xmm12, xmm8
	vcmpneq_ussd k2, xmm9, xmm15
%endif
