	vcmpnle_uqss xmm1, xmm0, qword [0xbd1]
	vcmpnle_uqss xmm1, qword [0xbd1]
	vcmpnle_uqss xmm5, xmm2, xmm7
	vcmpnle_uqss k2, xmm0, dword [0xbb5]
	vcmpnle_uqss k5, xmm6, xmm2

%ifdef ERROR
	vcmpnle_uqss xmm10, xmm11, xmm12
	vcmpnle_uqss k3, xmm13, xmm11
%endif
