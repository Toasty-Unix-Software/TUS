	vcmpnless xmm1, xmm7, xmm0
	vcmpnless xmm1, xmm0
	vcmpnless xmm0, xmm6, xmm1
	vcmpnless k1, xmm6, dword [0x9da]
	vcmpnless k6, xmm4, dword [0x54a]

%ifdef ERROR
	vcmpnless xmm9, xmm12, xmm9
	vcmpnless k3, xmm13, xmm12
%endif
