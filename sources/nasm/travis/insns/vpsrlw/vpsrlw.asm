default rel
	vpsrlw xmm7, xmm2, xmm0
	vpsrlw xmm7, xmm0
	vpsrlw xmm1, xmm4, xmm6
	vpsrlw xmm1, xmm0, 0x4b
	vpsrlw xmm1, 0x4b
	vpsrlw xmm5, xmm6, 0x46
	vpsrlw ymm1, ymm4, oword [0xa98]
	vpsrlw ymm1, oword [0xa98]
	vpsrlw ymm1, ymm0, xmm2
	vpsrlw ymm2, ymm4, 0x42
	vpsrlw ymm2, 0x42
	vpsrlw ymm0, ymm6, 0x37
	vpsrlw xmm14, xmm14, xmm13
	vpsrlw xmm11, xmm13, 0x42
	vpsrlw ymm14, ymm14, xmm10
	vpsrlw ymm13, ymm13, 0xf2
	vpsrlw xmm30, xmm16, xmm20
	vpsrlw xmm17, xmm30, 0x34
	vpsrlw ymm31, ymm27, xmm23
	vpsrlw ymm20, ymm16, 0x2c
	vpsrlw xmm2{k6}, xmm6, oword [0xd19]
	vpsrlw ymm3{k7}, ymm3, xmm0
	vpsrlw zmm2{k5}, zmm4, oword [0x339]
	vpsrlw xmm0{k5}, xmm1, 0xc5
	vpsrlw ymm1{k7}, yword [0xd22], 0xed
	vpsrlw zmm2{k3}, zword [0x1bf], 0x5b
	vpsrlw xmm4{k7}{z}, xmm2, oword [0xcb0]
	vpsrlw ymm1{k2}{z}, ymm7, xmm6
	vpsrlw zmm5{k6}{z}, zmm1, xmm5
	vpsrlw xmm5{k6}{z}, oword [0xba4], 0xb0
	vpsrlw ymm4{k1}{z}, ymm6, 0xa5
	vpsrlw zmm1{k3}{z}, zmm6, 0x78
	vpsrlw xmm2, xmm0, oword [eax+1]
	vpsrlw xmm2, xmm2, oword [eax+64]
	vpsrlw ymm5, ymm1, oword [eax+1]
	vpsrlw ymm2, ymm2, oword [eax+64]
	vpsrlw xmm4, xmm4, oword [eax+1]
	vpsrlw xmm2, xmm2, oword [eax+64]
	vpsrlw ymm2, ymm6, oword [eax+1]
	vpsrlw ymm1, ymm6, oword [eax+64]
	vpsrlw zmm2, zmm2, oword [eax+1]
	vpsrlw zmm5, zmm1, oword [eax+64]
	vpsrlw xmm1, oword [eax+1], 0x24
	vpsrlw xmm5, oword [eax+64], 0xf2
	vpsrlw ymm2, yword [eax+1], 0x4e
	vpsrlw ymm6, yword [eax+64], 0xd7
	vpsrlw zmm0, zword [eax+1], 0xb
	vpsrlw zmm2, zword [eax+64], 0xc
	vpsrlw xmm7, xmm0, [0x7cc]
	vpsrlw ymm3, ymm5, [0xeb9]
	vpsrlw xmm3, xmm2, [0x5ae]
	vpsrlw ymm5, ymm2, [0x589]
	vpsrlw zmm6, zmm6, [0x961]
	vpsrlw xmm7, [0xbdd], 0x42
	vpsrlw ymm2, [0xbff], 0x21
	vpsrlw zmm0, [0x75c], 0x12
