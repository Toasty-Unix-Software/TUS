default rel
	vcmple_osss xmm0, xmm1, xmm6
	vcmple_osss xmm0, xmm6
	vcmple_osss xmm2, xmm0, xmm4
	vcmple_osss k2, xmm4, xmm0
	vcmple_osss k3, xmm0, xmm6
	vcmple_osss xmm12, xmm8, xmm8
	vcmple_osss k5, xmm13, xmm13
	vcmple_osss k7{k6}, xmm7, xmm1
	vcmple_osss k5, xmm1, xmm5, {sae}
	vcmple_osss xmm7, xmm1, qword [eax+1]
	vcmple_osss xmm1, xmm5, qword [eax+64]
	vcmple_osss k3, xmm1, dword [eax+1]
	vcmple_osss k6, xmm4, dword [eax+64]
	vcmple_osss xmm4, xmm4, [0x64b]
	vcmple_osss k2, xmm1, [0xef6]
