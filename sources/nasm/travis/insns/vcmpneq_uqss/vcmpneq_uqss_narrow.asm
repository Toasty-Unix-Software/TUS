	vcmpneq_uqss xmm3, xmm1, xmm1
	vcmpneq_uqss xmm3, xmm1
	vcmpneq_uqss xmm3, xmm6, xmm7
	vcmpneq_uqss k7, xmm1, xmm0
	vcmpneq_uqss k4, xmm5, dword [0xd38]

%ifdef ERROR
	vcmpneq_uqss xmm10, xmm9, xmm12
	vcmpneq_uqss k3, xmm9, xmm10
%endif
