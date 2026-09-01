default rel
	vcmpngt_uqsd xmm1, xmm4, xmm3
	vcmpngt_uqsd xmm1, xmm3
	vcmpngt_uqsd xmm1, xmm2, xmm5
	vcmpngt_uqsd k7, xmm5, xmm2
	vcmpngt_uqsd k3, xmm5, xmm1
	vcmpngt_uqsd xmm13, xmm8, xmm15
	vcmpngt_uqsd k5, xmm15, xmm13
	vcmpngt_uqsd k7{k5}, xmm6, qword [0xcb9]
	vcmpngt_uqsd k5, xmm4, xmm5, {sae}
	vcmpngt_uqsd xmm5, xmm4, qword [eax+1]
	vcmpngt_uqsd xmm4, xmm4, qword [eax+64]
	vcmpngt_uqsd k5, xmm4, qword [eax+1]
	vcmpngt_uqsd k5, xmm5, qword [eax+64]
	vcmpngt_uqsd xmm3, xmm7, [0xb3d]
	vcmpngt_uqsd k1, xmm2, [0x562]
