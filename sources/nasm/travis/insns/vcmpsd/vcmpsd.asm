default rel
	vcmpsd xmm6, xmm6, qword [0x8ea], 0x5b
	vcmpsd xmm6, qword [0x8ea], 0x5b
	vcmpsd xmm2, xmm2, xmm0, 0x5
	vcmpsd k5, xmm7, xmm7, 0x5e
	vcmpsd k6, xmm5, qword [0x2d6], 0xe8
	vcmpsd xmm13, xmm10, xmm8, 0x8b
	vcmpsd k7, xmm12, xmm11, 0x51
	vcmpsd k6{k7}, xmm2, xmm6, 0x1f
	vcmpsd xmm7, xmm4, qword [eax+1], 0x47
	vcmpsd xmm7, xmm1, qword [eax+64], 0x2f
	vcmpsd k7, xmm3, qword [eax+1], 0xf
	vcmpsd k7, xmm7, qword [eax+64], 0xb7
	vcmpsd xmm0, xmm0, [0x356], 0x12
	vcmpsd k1, xmm0, [0xd74], 0x57
