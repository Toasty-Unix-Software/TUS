default rel
	vfpclassph k6, xmm0, 0x18
	vfpclassph k1, xmm7, 0x35
	vfpclassph k1, ymm2, 0x43
	vfpclassph k7, ymm6, 0xec
	vfpclassph k6, zmm5, 0xdf
	vfpclassph k5, zmm2, 0x4c
	vfpclassph k6, xmm15, 0xb7
	vfpclassph k4, ymm11, 0xbc
	vfpclassph k4, zmm11, 0x7f
	vfpclassph k6, xmm21, 0xbb
	vfpclassph k6, ymm22, 0x7e
	vfpclassph k1, zmm26, 0xbc
	vfpclassph k2{k4}, xmm7, 0x7f
	vfpclassph k2{k6}, ymm6, 0xbd
	vfpclassph k6{k7}, zmm6, 0x66
	vfpclassph k6, oword [eax+1], 0xc
	vfpclassph k4, oword [eax+64], 0xd6
	vfpclassph k3, yword [eax+1], 0xf6
	vfpclassph k7, yword [eax+64], 0xf5
	vfpclassph k7, zword [eax+1], 0xed
	vfpclassph k1, zword [eax+64], 0x5c
	vfpclassph k7, [0x272], 0xfa
	vfpclassph k3, [0x466], 0xc1
	vfpclassph k2, [0x29d], 0x61
