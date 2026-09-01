default rel
	vcmpeq_oqsd k2, xmm7, xmm1
	vcmpeq_oqsd k2, xmm1, xmm4
	vcmpeq_oqsd k4, xmm11, xmm9
	vcmpeq_oqsd k1, xmm21, xmm17
	vcmpeq_oqsd k6{k2}, xmm5, xmm2
	vcmpeq_oqsd k6, xmm5, xmm7, {sae}
	vcmpeq_oqsd k2, xmm2, qword [eax+1]
	vcmpeq_oqsd k5, xmm4, qword [eax+64]
	vcmpeq_oqsd k6, xmm7, [0xdbf]
