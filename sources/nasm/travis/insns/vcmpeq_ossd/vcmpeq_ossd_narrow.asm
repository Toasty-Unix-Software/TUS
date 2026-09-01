	vcmpeq_ossd xmm7, xmm4, xmm3
	vcmpeq_ossd xmm7, xmm3
	vcmpeq_ossd xmm3, xmm4, qword [0x390]
	vcmpeq_ossd k1, xmm2, xmm6
	vcmpeq_ossd k5, xmm7, xmm6

%ifdef ERROR
	vcmpeq_ossd xmm14, xmm10, xmm13
	vcmpeq_ossd k3, xmm15, xmm10
%endif
