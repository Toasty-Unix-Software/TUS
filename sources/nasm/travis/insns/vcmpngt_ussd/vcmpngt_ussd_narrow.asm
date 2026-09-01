	vcmpngt_ussd xmm5, xmm1, xmm6
	vcmpngt_ussd xmm5, xmm6
	vcmpngt_ussd xmm3, xmm4, qword [0x92e]
	vcmpngt_ussd k6, xmm0, xmm5
	vcmpngt_ussd k7, xmm6, xmm3

%ifdef ERROR
	vcmpngt_ussd xmm10, xmm11, xmm10
	vcmpngt_ussd k2, xmm12, xmm8
%endif
