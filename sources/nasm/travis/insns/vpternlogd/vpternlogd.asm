default rel
	vpternlogd xmm5, xmm4, xmm1, 0x3a
	vpternlogd xmm0, xmm7, xmm1, 0xb7
	vpternlogd ymm6, ymm5, ymm7, 0x1b
	vpternlogd ymm1, ymm4, ymm3, 0xa1
	vpternlogd zmm6, zmm4, zword [0x8fe], 0x39
	vpternlogd zmm4, zmm6, zword [0x83f], 0xd6
	vpternlogd xmm14, xmm11, xmm11, 0x9a
	vpternlogd ymm13, ymm12, ymm9, 0x7
	vpternlogd zmm12, zmm14, zmm10, 0xaf
	vpternlogd xmm27, xmm23, xmm21, 0xcb
	vpternlogd ymm28, ymm20, ymm30, 0xd9
	vpternlogd zmm18, zmm29, zmm26, 0x79
	vpternlogd xmm4{k4}, xmm7, xmm1, 0xdc
	vpternlogd ymm1{k7}, ymm5, ymm0, 0x92
	vpternlogd zmm2{k5}, zmm4, zmm1, 0x27
	vpternlogd xmm4{k2}{z}, xmm3, oword [0xde5], 0x93
	vpternlogd ymm7{k6}{z}, ymm0, ymm1, 0x5
	vpternlogd zmm3{k7}{z}, zmm6, zmm3, 0x38
	vpternlogd xmm3, xmm0, oword [eax+1], 0x2f
	vpternlogd xmm1, xmm4, oword [eax+64], 0x15
	vpternlogd ymm2, ymm5, yword [eax+1], 0x9e
	vpternlogd ymm2, ymm6, yword [eax+64], 0xf6
	vpternlogd zmm4, zmm3, zword [eax+1], 0x39
	vpternlogd zmm4, zmm6, zword [eax+64], 0xfa
	vpternlogd xmm1, xmm5, [0xb83], 0x7
	vpternlogd ymm4, ymm5, [0xb44], 0x35
	vpternlogd zmm0, zmm1, [0xa49], 0x50
