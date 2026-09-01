	vcmpngt_uqss xmm2, xmm4, xmm4
	vcmpngt_uqss xmm2, xmm4
	vcmpngt_uqss xmm0, xmm5, qword [0x7f7]
	vcmpngt_uqss k1, xmm2, xmm0
	vcmpngt_uqss k1, xmm2, dword [0x900]

%ifdef ERROR
	vcmpngt_uqss xmm8, xmm13, xmm14
	vcmpngt_uqss k1, xmm10, xmm15
%endif
