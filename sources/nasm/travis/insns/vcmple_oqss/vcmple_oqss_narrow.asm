	vcmple_oqss xmm1, xmm0, xmm4
	vcmple_oqss xmm1, xmm4
	vcmple_oqss xmm1, xmm7, qword [0x9ae]
	vcmple_oqss k7, xmm2, xmm3
	vcmple_oqss k5, xmm1, dword [0x41e]

%ifdef ERROR
	vcmple_oqss xmm11, xmm14, xmm13
	vcmple_oqss k3, xmm12, xmm14
%endif
