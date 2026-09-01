default rel
	vcmpeq_osss xmm6, xmm0, xmm3
	vcmpeq_osss xmm6, xmm3
	vcmpeq_osss xmm6, xmm7, xmm1
	vcmpeq_osss k2, xmm6, xmm4
	vcmpeq_osss k2, xmm1, xmm4
	vcmpeq_osss xmm10, xmm13, xmm13
	vcmpeq_osss k6, xmm15, xmm9
	vcmpeq_osss k3{k3}, xmm1, xmm7
	vcmpeq_osss k1, xmm5, xmm5, {sae}
	vcmpeq_osss xmm3, xmm4, qword [eax+1]
	vcmpeq_osss xmm3, xmm4, qword [eax+64]
	vcmpeq_osss k3, xmm0, dword [eax+1]
	vcmpeq_osss k6, xmm0, dword [eax+64]
	vcmpeq_osss xmm5, xmm7, [0xe60]
	vcmpeq_osss k1, xmm5, [0x1f1]
