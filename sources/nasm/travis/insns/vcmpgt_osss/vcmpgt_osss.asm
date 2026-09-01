default rel
	vcmpgt_osss xmm6, xmm5, qword [0xf31]
	vcmpgt_osss xmm6, qword [0xf31]
	vcmpgt_osss xmm3, xmm6, qword [0xa52]
	vcmpgt_osss k6, xmm6, xmm6
	vcmpgt_osss k4, xmm5, xmm2
	vcmpgt_osss xmm10, xmm12, xmm13
	vcmpgt_osss k2, xmm11, xmm11
	vcmpgt_osss k7{k5}, xmm6, xmm4
	vcmpgt_osss k7, xmm1, xmm1, {sae}
	vcmpgt_osss xmm1, xmm1, qword [eax+1]
	vcmpgt_osss xmm6, xmm5, qword [eax+64]
	vcmpgt_osss k2, xmm1, dword [eax+1]
	vcmpgt_osss k2, xmm6, dword [eax+64]
	vcmpgt_osss xmm1, xmm1, [0xea2]
	vcmpgt_osss k6, xmm7, [0x333]
