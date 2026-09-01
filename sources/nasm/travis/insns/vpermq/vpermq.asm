default rel
	vpermq ymm4, ymm3, 0x6e
	vpermq ymm7, yword [0xadf], 0xed
	vpermq ymm1, ymm3, 0x29
	vpermq ymm3, ymm2, 0x33
	vpermq zmm7, zmm6, 0xcc
	vpermq zmm7, zword [0xfd7], 0xa
	vpermq ymm0, ymm4, yword [0xaa0]
	vpermq ymm0, yword [0xaa0]
	vpermq ymm1, ymm4, yword [0x4d5]
	vpermq ymm9, ymm12, 0xf7
	vpermq ymm15, ymm11, 0x29
	vpermq zmm12, zmm11, 0x95
	vpermq ymm14, ymm10, ymm15
	vpermq ymm18, ymm20, 0x2b
	vpermq ymm19, ymm25, 0xee
	vpermq zmm18, zmm25, 0x4f
	vpermq ymm21, ymm26, ymm20
	vpermq ymm3{k7}, ymm6, 0xb2
	vpermq zmm2{k4}, zmm3, 0xf6
	vpermq ymm6{k7}, ymm1, ymm2
	vpermq zmm6{k6}, zmm6, zword [0xbb1]
	vpermq ymm1{k4}{z}, yword [0x649], 0x88
	vpermq zmm1{k3}{z}, zmm2, 0xd0
	vpermq ymm4{k5}{z}, ymm2, ymm2
	vpermq zmm0{k2}{z}, zmm0, zmm0
	vpermq ymm0, yword [eax+1], 0xff
	vpermq ymm0, yword [eax+64], 0x70
	vpermq ymm4, yword [eax+1], 0x96
	vpermq ymm7, yword [eax+64], 0x12
	vpermq zmm2, zword [eax+1], 0x54
	vpermq zmm4, zword [eax+64], 0x59
	vpermq ymm6, ymm4, yword [eax+1]
	vpermq ymm3, ymm0, yword [eax+64]
	vpermq zmm5, zmm6, zword [eax+1]
	vpermq zmm1, zmm2, zword [eax+64]
	vpermq ymm3, [0x352], 0xa0
	vpermq ymm0, [0xc95], 0x42
	vpermq zmm4, [0xc11], 0x1e
	vpermq ymm6, ymm6, [0xded]
	vpermq zmm1, zmm3, [0xe9f]
