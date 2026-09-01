	vcmpnge_uqss xmm7, xmm7, xmm1
	vcmpnge_uqss xmm7, xmm1
	vcmpnge_uqss xmm6, xmm3, qword [0xe32]
	vcmpnge_uqss k4, xmm3, dword [0xc5a]
	vcmpnge_uqss k3, xmm4, dword [0xdde]

%ifdef ERROR
	vcmpnge_uqss xmm10, xmm15, xmm8
	vcmpnge_uqss k3, xmm12, xmm9
%endif
