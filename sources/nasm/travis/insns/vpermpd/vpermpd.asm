default rel
	vpermpd ymm0, ymm5, 0xb6
	vpermpd ymm2, ymm2, 0xd7
	vpermpd ymm5, yword [0x9e8], 0x80
	vpermpd ymm2, ymm3, 0xd0
	vpermpd zmm3, zword [0x8f7], 0xb
	vpermpd zmm5, zmm5, 0x50
	vpermpd ymm0, ymm4, ymm1
	vpermpd ymm0, ymm1
	vpermpd ymm3, ymm0, ymm6
	vpermpd ymm11, ymm10, 0x79
	vpermpd ymm9, ymm10, 0x52
	vpermpd zmm9, zmm14, 0xc7
	vpermpd ymm9, ymm14, ymm9
	vpermpd ymm25, ymm19, 0x63
	vpermpd ymm28, ymm27, 0x31
	vpermpd zmm23, zmm27, 0x73
	vpermpd ymm19, ymm20, ymm28
	vpermpd ymm7{k4}, ymm5, 0x7b
	vpermpd zmm3{k6}, zword [0xa41], 0x53
	vpermpd ymm6{k4}, ymm1, ymm4
	vpermpd zmm0{k6}, zmm6, zmm6
	vpermpd ymm6{k6}{z}, ymm1, 0x9
	vpermpd zmm5{k7}{z}, zword [0xe62], 0x4e
	vpermpd ymm6{k4}{z}, ymm1, ymm0
	vpermpd zmm3{k7}{z}, zmm7, zword [0xaa3]
	vpermpd ymm4, yword [eax+1], 0xfd
	vpermpd ymm4, yword [eax+64], 0x6b
	vpermpd ymm5, yword [eax+1], 0xf7
	vpermpd ymm6, yword [eax+64], 0x8
	vpermpd zmm1, zword [eax+1], 0x2d
	vpermpd zmm0, zword [eax+64], 0xc5
	vpermpd ymm2, ymm4, yword [eax+1]
	vpermpd ymm1, ymm4, yword [eax+64]
	vpermpd zmm1, zmm3, zword [eax+1]
	vpermpd zmm4, zmm7, zword [eax+64]
	vpermpd ymm1, [0x24b], 0xdd
	vpermpd ymm1, [0xb9b], 0xc9
	vpermpd zmm1, [0xe01], 0xaf
	vpermpd ymm5, ymm6, [0xb1a]
	vpermpd zmm7, zmm6, [0x1e3]
