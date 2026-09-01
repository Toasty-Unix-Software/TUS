	vcmpnlt_uqss xmm3, xmm2, qword [0x660]
	vcmpnlt_uqss xmm3, qword [0x660]
	vcmpnlt_uqss xmm3, xmm2, qword [0xa14]
	vcmpnlt_uqss k3, xmm1, xmm6
	vcmpnlt_uqss k6, xmm2, xmm1

%ifdef ERROR
	vcmpnlt_uqss xmm14, xmm12, xmm9
	vcmpnlt_uqss k5, xmm9, xmm10
%endif
