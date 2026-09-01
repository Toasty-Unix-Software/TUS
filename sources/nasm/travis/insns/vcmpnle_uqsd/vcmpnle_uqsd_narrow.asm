	vcmpnle_uqsd xmm0, xmm0, xmm1
	vcmpnle_uqsd xmm0, xmm1
	vcmpnle_uqsd xmm7, xmm5, qword [0xc82]
	vcmpnle_uqsd k4, xmm2, qword [0xc74]
	vcmpnle_uqsd k2, xmm0, xmm3

%ifdef ERROR
	vcmpnle_uqsd xmm8, xmm10, xmm13
	vcmpnle_uqsd k7, xmm10, xmm13
%endif
