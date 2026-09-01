default rel
	vcmpss xmm1, xmm5, dword [0x40b], 0xbf
	vcmpss xmm1, dword [0x40b], 0xbf
	vcmpss xmm0, xmm0, dword [0x62d], 0xe
	vcmpss k1, xmm1, xmm2, 0x67
	vcmpss k6, xmm5, xmm7, 0xa1
	vcmpss xmm13, xmm11, xmm12, 0x62
	vcmpss k7, xmm8, xmm13, 0xc9
	vcmpss k6{k6}, xmm4, xmm3, 0x2d
	vcmpss xmm6, xmm5, dword [eax+1], 0x1a
	vcmpss xmm4, xmm4, dword [eax+64], 0x25
	vcmpss k1, xmm0, dword [eax+1], 0x39
	vcmpss k5, xmm7, dword [eax+64], 0x37
	vcmpss xmm6, xmm3, [0x889], 0x5
	vcmpss k4, xmm6, [0x4eb], 0x77
