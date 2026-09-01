default rel
	vexpandpd xmm0, oword [0x92b]
	vexpandpd xmm5, oword [0xedb]
	vexpandpd ymm6, yword [0x778]
	vexpandpd ymm2, yword [0x728]
	vexpandpd zmm5, zword [0x538]
	vexpandpd zmm0, zword [0x677]
	vexpandpd xmm1, xmm2
	vexpandpd xmm6, xmm0
	vexpandpd xmm11, oword [0x51f]
	vexpandpd ymm13, yword [0xeae]
	vexpandpd zmm13, zword [0xd75]
	vexpandpd xmm10, xmm11
	vexpandpd xmm30, oword [0x2c2]
	vexpandpd ymm23, yword [0xb8a]
	vexpandpd zmm25, zword [0xd92]
	vexpandpd xmm17, xmm18
	vexpandpd xmm4{k2}, oword [0x7ed]
	vexpandpd ymm1{k3}, yword [0x85c]
	vexpandpd zmm6{k7}, zword [0x338]
	vexpandpd xmm0{k5}, xmm0
	vexpandpd ymm4{k4}, ymm3
	vexpandpd zmm2{k4}, zmm1
	vexpandpd xmm4{k4}{z}, oword [0x8a9]
	vexpandpd ymm0{k7}{z}, yword [0x50a]
	vexpandpd zmm2{k3}{z}, zword [0x760]
	vexpandpd xmm7{k7}{z}, xmm3
	vexpandpd ymm0{k6}{z}, ymm0
	vexpandpd zmm7{k4}{z}, zmm6
	vexpandpd xmm7, oword [eax+1]
	vexpandpd xmm2, oword [eax+64]
	vexpandpd ymm7, yword [eax+1]
	vexpandpd ymm4, yword [eax+64]
	vexpandpd zmm1, zword [eax+1]
	vexpandpd zmm0, zword [eax+64]
	vexpandpd xmm4, [0x5cb]
	vexpandpd ymm0, [0xb9d]
	vexpandpd zmm2, [0x7b5]
