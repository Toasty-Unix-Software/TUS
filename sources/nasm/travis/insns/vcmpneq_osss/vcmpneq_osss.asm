default rel
	vcmpneq_osss xmm7, xmm1, xmm6
	vcmpneq_osss xmm7, xmm6
	vcmpneq_osss xmm0, xmm3, xmm3
	vcmpneq_osss k3, xmm0, xmm1
	vcmpneq_osss k7, xmm0, xmm1
	vcmpneq_osss xmm9, xmm9, xmm15
	vcmpneq_osss k2, xmm9, xmm14
	vcmpneq_osss k5{k6}, xmm2, dword [0xb8f]
	vcmpneq_osss k4, xmm2, xmm0, {sae}
	vcmpneq_osss xmm7, xmm4, qword [eax+1]
	vcmpneq_osss xmm3, xmm3, qword [eax+64]
	vcmpneq_osss k6, xmm5, dword [eax+1]
	vcmpneq_osss k5, xmm5, dword [eax+64]
	vcmpneq_osss xmm7, xmm4, [0x705]
	vcmpneq_osss k3, xmm5, [0x8e7]
