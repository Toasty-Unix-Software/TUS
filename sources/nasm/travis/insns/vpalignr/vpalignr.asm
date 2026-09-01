default rel
	vpalignr xmm5, xmm7, xmm5, 0x2d
	vpalignr xmm5, xmm5, 0x2d
	vpalignr xmm7, xmm6, xmm3, 0x26
	vpalignr ymm2, ymm0, ymm3, 0xc4
	vpalignr ymm2, ymm3, 0xc4
	vpalignr ymm5, ymm6, yword [0xe72], 0xf1
	vpalignr xmm3, xmm2, xmm4, 0x41
	vpalignr xmm3, xmm4, 0x41
	vpalignr xmm6, xmm6, oword [0x5c3], 0xd1
	vpalignr ymm4, ymm7, yword [0x95d], 0xf6
	vpalignr ymm4, yword [0x95d], 0xf6
	vpalignr ymm4, ymm7, yword [0xb6c], 0xc6
	vpalignr xmm13, xmm14, xmm14, 0xad
	vpalignr ymm8, ymm9, ymm12, 0xad
	vpalignr xmm11, xmm14, xmm9, 0x28
	vpalignr ymm10, ymm12, ymm10, 0xc0
	vpalignr xmm30, xmm30, xmm23, 0xd9
	vpalignr ymm30, ymm24, ymm25, 0xbd
	vpalignr xmm27, xmm30, xmm16, 0xd6
	vpalignr ymm27, ymm16, ymm20, 0xab
	vpalignr xmm1{k7}, xmm6, oword [0x35c], 0xf8
	vpalignr ymm3{k1}, ymm3, ymm7, 0x5c
	vpalignr zmm5{k4}, zmm2, zmm1, 0x3e
	vpalignr xmm6{k5}{z}, xmm7, xmm1, 0xb3
	vpalignr ymm2{k6}{z}, ymm1, ymm5, 0x15
	vpalignr zmm2{k1}{z}, zmm3, zmm4, 0x4a
	vpalignr xmm0, xmm1, oword [eax+1], 0x91
	vpalignr xmm5, xmm3, oword [eax+64], 0x93
	vpalignr ymm0, ymm7, yword [eax+1], 0x93
	vpalignr ymm1, ymm0, yword [eax+64], 0x9d
	vpalignr xmm1, xmm4, oword [eax+1], 0x45
	vpalignr xmm5, xmm7, oword [eax+64], 0xfd
	vpalignr ymm5, ymm2, yword [eax+1], 0xf1
	vpalignr ymm3, ymm6, yword [eax+64], 0x7c
	vpalignr zmm1, zmm7, zword [eax+1], 0xb
	vpalignr zmm5, zmm5, zword [eax+64], 0x16
	vpalignr xmm5, xmm4, [0x52c], 0x31
	vpalignr ymm7, ymm7, [0x1bc], 0x90
	vpalignr xmm0, xmm7, [0x78f], 0xd8
	vpalignr ymm2, ymm5, [0x153], 0xab
	vpalignr zmm4, zmm1, [0x54f], 0xcb
