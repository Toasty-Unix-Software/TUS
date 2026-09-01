	vcmpneqss xmm3, xmm3, qword [0x5b2]
	vcmpneqss xmm3, qword [0x5b2]
	vcmpneqss xmm1, xmm1, xmm1
	vcmpneqss k3, xmm4, xmm1
	vcmpneqss k1, xmm2, xmm0

%ifdef ERROR
	vcmpneqss xmm12, xmm12, xmm15
	vcmpneqss k6, xmm9, xmm15
%endif
