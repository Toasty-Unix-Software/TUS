default rel
	vfpclasssd k2, xmm6, 0xc9
	vfpclasssd k1, xmm1, 0xbc
	vfpclasssd k5, xmm13, 0xc5
	vfpclasssd k6, xmm18, 0x7a
	vfpclasssd k1{k2}, qword [0xa79], 0x5c
	vfpclasssd k3, qword [eax+1], 0xbd
	vfpclasssd k6, qword [eax+64], 0xb0
	vfpclasssd k5, [0xbe7], 0xf5
