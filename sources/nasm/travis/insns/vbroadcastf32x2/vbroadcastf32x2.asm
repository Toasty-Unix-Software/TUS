default rel
	vbroadcastf32x2 ymm7, xmm6
	vbroadcastf32x2 ymm4, qword [0x5ba]
	vbroadcastf32x2 zmm3, xmm0
	vbroadcastf32x2 zmm3, xmm4
	vbroadcastf32x2 ymm14, xmm13
	vbroadcastf32x2 zmm15, xmm10
	vbroadcastf32x2 ymm16, xmm30
	vbroadcastf32x2 zmm26, xmm26
	vbroadcastf32x2 ymm6{k2}, xmm4
	vbroadcastf32x2 zmm3{k4}, qword [0x978]
	vbroadcastf32x2 ymm0{k6}{z}, qword [0xe0d]
	vbroadcastf32x2 zmm4{k1}{z}, xmm0
	vbroadcastf32x2 ymm1, qword [eax+1]
	vbroadcastf32x2 ymm3, qword [eax+64]
	vbroadcastf32x2 zmm5, qword [eax+1]
	vbroadcastf32x2 zmm1, qword [eax+64]
	vbroadcastf32x2 ymm1, [0x433]
	vbroadcastf32x2 zmm7, [0x552]
