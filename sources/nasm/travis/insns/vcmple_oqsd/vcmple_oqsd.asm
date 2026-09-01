default rel
	vcmple_oqsd xmm2, xmm0, xmm2
	vcmple_oqsd xmm2, xmm2
	vcmple_oqsd xmm1, xmm7, qword [0xdf6]
	vcmple_oqsd k1, xmm6, qword [0xac5]
	vcmple_oqsd k5, xmm6, qword [0xbb2]
	vcmple_oqsd xmm12, xmm15, xmm8
	vcmple_oqsd k5, xmm15, xmm8
	vcmple_oqsd k6{k5}, xmm1, xmm2
	vcmple_oqsd k1, xmm5, xmm7, {sae}
	vcmple_oqsd xmm2, xmm5, qword [eax+1]
	vcmple_oqsd xmm4, xmm4, qword [eax+64]
	vcmple_oqsd k2, xmm4, qword [eax+1]
	vcmple_oqsd k7, xmm6, qword [eax+64]
	vcmple_oqsd xmm2, xmm1, [0x668]
	vcmple_oqsd k7, xmm7, [0x8b6]
