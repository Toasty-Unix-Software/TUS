default rel
	vpsraw xmm0, xmm3, oword [0xb55]
	vpsraw xmm0, oword [0xb55]
	vpsraw xmm7, xmm3, oword [0xc77]
	vpsraw xmm2, xmm0, 0xc5
	vpsraw xmm2, 0xc5
	vpsraw xmm4, xmm1, 0x1b
	vpsraw ymm3, ymm7, xmm5
	vpsraw ymm3, xmm5
	vpsraw ymm5, ymm5, oword [0xfa2]
	vpsraw ymm5, ymm4, 0xa4
	vpsraw ymm5, 0xa4
	vpsraw ymm7, ymm0, 0xb9
	vpsraw xmm9, xmm8, xmm14
	vpsraw xmm14, xmm13, 0xfa
	vpsraw ymm13, ymm13, xmm15
	vpsraw ymm12, ymm9, 0x74
	vpsraw xmm17, xmm19, xmm17
	vpsraw xmm16, xmm30, 0xae
	vpsraw ymm21, ymm26, xmm29
	vpsraw ymm16, ymm26, 0x42
	vpsraw xmm5{k6}, xmm3, xmm4
	vpsraw ymm3{k7}, ymm3, xmm3
	vpsraw zmm1{k6}, zmm7, oword [0x101]
	vpsraw xmm0{k3}, xmm1, 0xc5
	vpsraw ymm6{k3}, ymm3, 0xdc
	vpsraw zmm4{k4}, zword [0x4b9], 0x89
	vpsraw xmm1{k2}{z}, xmm1, xmm2
	vpsraw ymm0{k4}{z}, ymm3, xmm5
	vpsraw zmm3{k4}{z}, zmm6, xmm5
	vpsraw xmm5{k5}{z}, oword [0x332], 0x7
	vpsraw ymm5{k4}{z}, ymm2, 0xb7
	vpsraw zmm3{k4}{z}, zword [0x6f5], 0xcf
	vpsraw xmm3, xmm6, oword [eax+1]
	vpsraw xmm5, xmm5, oword [eax+64]
	vpsraw ymm5, ymm3, oword [eax+1]
	vpsraw ymm2, ymm7, oword [eax+64]
	vpsraw xmm5, xmm4, oword [eax+1]
	vpsraw xmm3, xmm3, oword [eax+64]
	vpsraw ymm5, ymm5, oword [eax+1]
	vpsraw ymm2, ymm1, oword [eax+64]
	vpsraw zmm6, zmm3, oword [eax+1]
	vpsraw zmm4, zmm5, oword [eax+64]
	vpsraw xmm4, oword [eax+1], 0x8
	vpsraw xmm5, oword [eax+64], 0x6
	vpsraw ymm1, yword [eax+1], 0x33
	vpsraw ymm3, yword [eax+64], 0x7c
	vpsraw zmm4, zword [eax+1], 0xd7
	vpsraw zmm3, zword [eax+64], 0x96
	vpsraw xmm3, xmm0, [0x475]
	vpsraw ymm0, ymm1, [0x649]
	vpsraw xmm3, xmm7, [0x23f]
	vpsraw ymm7, ymm7, [0xba5]
	vpsraw zmm7, zmm3, [0x7f4]
	vpsraw xmm4, [0x186], 0x1e
	vpsraw ymm0, [0xa63], 0x5e
	vpsraw zmm2, [0x689], 0xac
