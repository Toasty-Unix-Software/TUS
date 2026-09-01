default rel
	vprorvd xmm6, xmm0, xmm5
	vprorvd xmm6, xmm5
	vprorvd xmm0, xmm1, xmm0
	vprorvd ymm4, ymm3, ymm6
	vprorvd ymm4, ymm6
	vprorvd ymm4, ymm3, ymm2
	vprorvd zmm2, zmm3, zmm1
	vprorvd zmm2, zmm1
	vprorvd zmm1, zmm2, zmm3
	vprorvd xmm10, xmm11, xmm12
	vprorvd ymm13, ymm8, ymm13
	vprorvd zmm15, zmm15, zmm15
	vprorvd xmm16, xmm27, xmm22
	vprorvd ymm31, ymm29, ymm19
	vprorvd zmm26, zmm16, zmm20
	vprorvd xmm1{k6}, xmm3, xmm2
	vprorvd ymm0{k6}, ymm1, ymm7
	vprorvd zmm0{k6}, zmm1, zmm0
	vprorvd xmm0{k7}{z}, xmm1, oword [0x313]
	vprorvd ymm7{k3}{z}, ymm0, ymm1
	vprorvd zmm0{k3}{z}, zmm3, zword [0xb9f]
	vprorvd xmm6, xmm7, oword [eax+1]
	vprorvd xmm6, xmm1, oword [eax+64]
	vprorvd ymm0, ymm3, yword [eax+1]
	vprorvd ymm5, ymm6, yword [eax+64]
	vprorvd zmm7, zmm2, zword [eax+1]
	vprorvd zmm5, zmm5, zword [eax+64]
	vprorvd xmm5, xmm4, [0xdfe]
	vprorvd ymm0, ymm1, [0x4bf]
	vprorvd zmm3, zmm5, [0xc75]
