default rel
	vbroadcasti32x2 xmm7, xmm0
	vbroadcasti32x2 xmm0, qword [0x3b1]
	vbroadcasti32x2 ymm1, qword [0x6c3]
	vbroadcasti32x2 ymm3, xmm4
	vbroadcasti32x2 zmm3, xmm6
	vbroadcasti32x2 zmm2, xmm5
	vbroadcasti32x2 xmm15, xmm8
	vbroadcasti32x2 ymm15, xmm13
	vbroadcasti32x2 zmm8, xmm9
	vbroadcasti32x2 xmm18, xmm24
	vbroadcasti32x2 ymm23, xmm27
	vbroadcasti32x2 zmm24, xmm28
	vbroadcasti32x2 xmm7{k3}, qword [0xc3d]
	vbroadcasti32x2 ymm2{k3}, xmm5
	vbroadcasti32x2 zmm3{k4}, qword [0x61d]
	vbroadcasti32x2 xmm7{k5}{z}, qword [0x2ed]
	vbroadcasti32x2 ymm2{k1}{z}, xmm2
	vbroadcasti32x2 zmm6{k5}{z}, xmm4
	vbroadcasti32x2 xmm5, qword [eax+1]
	vbroadcasti32x2 xmm0, qword [eax+64]
	vbroadcasti32x2 ymm2, qword [eax+1]
	vbroadcasti32x2 ymm1, qword [eax+64]
	vbroadcasti32x2 zmm1, qword [eax+1]
	vbroadcasti32x2 zmm4, qword [eax+64]
	vbroadcasti32x2 xmm6, [0x912]
	vbroadcasti32x2 ymm3, [0xaab]
	vbroadcasti32x2 zmm7, [0x729]
