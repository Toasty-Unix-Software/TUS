default rel
	vpmovsqd xmm4, xmm0
	vpmovsqd xmm1, xmm6
	vpmovsqd xmm5, ymm5
	vpmovsqd xmm6, ymm5
	vpmovsqd ymm6, zmm2
	vpmovsqd ymm1, zmm0
	vpmovsqd qword [0xe33], xmm2
	vpmovsqd qword [0xd78], xmm5
	vpmovsqd xmm10, xmm13
	vpmovsqd xmm11, ymm10
	vpmovsqd ymm10, zmm8
	vpmovsqd qword [0x8a9], xmm9
	vpmovsqd xmm30, xmm21
	vpmovsqd xmm31, ymm22
	vpmovsqd ymm19, zmm16
	vpmovsqd qword [0x1b0], xmm26
	vpmovsqd xmm2{k4}, xmm1
	vpmovsqd xmm7{k4}, ymm0
	vpmovsqd ymm7{k1}, zmm5
	vpmovsqd qword [0x72b]{k6}, xmm0
	vpmovsqd oword [0xae5]{k5}, ymm4
	vpmovsqd yword [0x134]{k1}, zmm4
	vpmovsqd qword [eax+1], xmm4
	vpmovsqd qword [eax+64], xmm7
	vpmovsqd oword [eax+1], ymm5
	vpmovsqd oword [eax+64], ymm7
	vpmovsqd yword [eax+1], zmm2
	vpmovsqd yword [eax+64], zmm2
	vpmovsqd [0x1b6], xmm3
	vpmovsqd [0x12b], ymm6
	vpmovsqd [0xe0b], zmm3
