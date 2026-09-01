	vcmpnle_ussd xmm3, xmm7, xmm4
	vcmpnle_ussd xmm3, xmm4
	vcmpnle_ussd xmm0, xmm3, xmm0
	vcmpnle_ussd k6, xmm2, xmm6
	vcmpnle_ussd k7, xmm4, xmm6

%ifdef ERROR
	vcmpnle_ussd xmm14, xmm8, xmm10
	vcmpnle_ussd k1, xmm15, xmm12
%endif
