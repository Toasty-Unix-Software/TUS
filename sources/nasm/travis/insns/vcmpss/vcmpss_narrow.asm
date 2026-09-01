	vcmpss xmm1, xmm5, dword [0x40b], 0xbf
	vcmpss xmm1, dword [0x40b], 0xbf
	vcmpss xmm0, xmm0, dword [0x62d], 0xe
	vcmpss k1, xmm1, xmm2, 0x67
	vcmpss k6, xmm5, xmm7, 0xa1

%ifdef ERROR
	vcmpss xmm13, xmm11, xmm12, 0x62
	vcmpss k7, xmm8, xmm13, 0xc9
%endif
