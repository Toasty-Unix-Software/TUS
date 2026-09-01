default rel
	vcmpgtsd xmm1, xmm6, qword [0xf5a]
	vcmpgtsd xmm1, qword [0xf5a]
	vcmpgtsd xmm0, xmm7, xmm0
	vcmpgtsd k4, xmm1, xmm3
	vcmpgtsd k5, xmm4, qword [0x514]
	vcmpgtsd xmm12, xmm10, xmm11
	vcmpgtsd k4, xmm15, xmm11
	vcmpgtsd k5{k6}, xmm2, qword [0xa0f]
	vcmpgtsd k2, xmm3, xmm0, {sae}
	vcmpgtsd xmm4, xmm3, qword [eax+1]
	vcmpgtsd xmm4, xmm7, qword [eax+64]
	vcmpgtsd k7, xmm0, qword [eax+1]
	vcmpgtsd k7, xmm3, qword [eax+64]
	vcmpgtsd xmm7, xmm3, [0xebf]
	vcmpgtsd k1, xmm3, [0x198]
