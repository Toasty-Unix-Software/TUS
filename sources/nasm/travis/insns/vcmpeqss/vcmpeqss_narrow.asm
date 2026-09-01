	vcmpeqss xmm6, xmm4, xmm1
	vcmpeqss xmm6, xmm1
	vcmpeqss xmm4, xmm4, qword [0xe78]
	vcmpeqss k1, xmm2, dword [0xe3d]
	vcmpeqss k5, xmm7, dword [0x4d6]

%ifdef ERROR
	vcmpeqss xmm10, xmm10, xmm14
	vcmpeqss k5, xmm12, xmm13
%endif
