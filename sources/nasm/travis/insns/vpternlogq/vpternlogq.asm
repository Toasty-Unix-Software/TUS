default rel
	vpternlogq xmm7, xmm2, xmm5, 0x55
	vpternlogq xmm7, xmm7, xmm1, 0xd1
	vpternlogq ymm4, ymm2, yword [0x290], 0x7d
	vpternlogq ymm6, ymm4, ymm7, 0x66
	vpternlogq zmm0, zmm6, zmm2, 0x25
	vpternlogq zmm6, zmm6, zmm1, 0x3
	vpternlogq xmm9, xmm9, xmm13, 0x1a
	vpternlogq ymm12, ymm12, ymm14, 0xd7
	vpternlogq zmm10, zmm11, zmm9, 0x4d
	vpternlogq xmm19, xmm29, xmm28, 0x2a
	vpternlogq ymm29, ymm16, ymm18, 0xa4
	vpternlogq zmm18, zmm20, zmm21, 0x89
	vpternlogq xmm0{k6}, xmm3, oword [0x297], 0x2f
	vpternlogq ymm3{k4}, ymm3, ymm6, 0x46
	vpternlogq zmm4{k7}, zmm4, zmm3, 0x68
	vpternlogq xmm0{k7}{z}, xmm7, xmm0, 0x80
	vpternlogq ymm2{k5}{z}, ymm1, yword [0xad5], 0xc2
	vpternlogq zmm1{k2}{z}, zmm6, zword [0xd71], 0x63
	vpternlogq xmm3, xmm4, oword [eax+1], 0x37
	vpternlogq xmm5, xmm7, oword [eax+64], 0xed
	vpternlogq ymm7, ymm0, yword [eax+1], 0xf4
	vpternlogq ymm1, ymm0, yword [eax+64], 0xfe
	vpternlogq zmm7, zmm5, zword [eax+1], 0x78
	vpternlogq zmm1, zmm7, zword [eax+64], 0x70
	vpternlogq xmm5, xmm6, [0xb53], 0x2e
	vpternlogq ymm4, ymm0, [0x2fe], 0x61
	vpternlogq zmm1, zmm5, [0x73e], 0xe0
