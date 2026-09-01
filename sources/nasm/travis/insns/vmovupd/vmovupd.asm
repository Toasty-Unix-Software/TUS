default rel
	vmovupd xmm0, xmm0
	vmovupd xmm6, oword [0x19a]
	vmovupd xmm3, xmm4
	vmovupd xmm6, xmm1
	vmovupd ymm0, ymm5
	vmovupd ymm2, ymm4
	vmovupd ymm2, ymm3
	vmovupd ymm2, ymm3
	vmovupd xmm8, xmm13
	vmovupd xmm9, xmm14
	vmovupd ymm9, ymm15
	vmovupd ymm12, ymm8
	vmovupd xmm30, xmm22
	vmovupd xmm21, xmm23
	vmovupd ymm17, ymm22
	vmovupd ymm20, ymm26
	vmovupd xmm0{k7}, xmm6
	vmovupd ymm2{k4}, ymm4
	vmovupd zmm7{k4}, zmm0
	vmovupd xmm5{k1}, xmm4
	vmovupd ymm6{k4}, ymm0
	vmovupd zmm1{k7}, zmm7
	vmovupd oword [0x863]{k3}, xmm7
	vmovupd yword [0xd28]{k4}, ymm7
	vmovupd zword [0xe47]{k1}, zmm4
	vmovupd xmm4, oword [eax+1]
	vmovupd xmm1, oword [eax+64]
	vmovupd oword [eax+1], xmm6
	vmovupd oword [eax+64], xmm3
	vmovupd ymm6, yword [eax+1]
	vmovupd ymm2, yword [eax+64]
	vmovupd yword [eax+1], ymm3
	vmovupd yword [eax+64], ymm5
	vmovupd xmm2, oword [eax+1]
	vmovupd xmm4, oword [eax+64]
	vmovupd ymm6, yword [eax+1]
	vmovupd ymm7, yword [eax+64]
	vmovupd zmm1, zword [eax+1]
	vmovupd zmm1, zword [eax+64]
	vmovupd oword [eax+1], xmm6
	vmovupd oword [eax+64], xmm1
	vmovupd yword [eax+1], ymm5
	vmovupd yword [eax+64], ymm2
	vmovupd zword [eax+1], zmm6
	vmovupd zword [eax+64], zmm3
	vmovupd xmm6, [0xaac]
	vmovupd [0x4d3], xmm6
	vmovupd ymm2, [0xd9f]
	vmovupd [0x9dc], ymm0
	vmovupd xmm3, [0x35e]
	vmovupd ymm3, [0xee2]
	vmovupd zmm5, [0xa34]
	vmovupd [0x1be], xmm3
	vmovupd [0x29d], ymm3
	vmovupd [0x407], zmm6
