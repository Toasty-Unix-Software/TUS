default rel
	vmovapd xmm6, oword [0xf62]
	vmovapd xmm5, xmm4
	vmovapd oword [0x18c], xmm3
	vmovapd xmm2, xmm6
	vmovapd ymm3, ymm7
	vmovapd ymm1, ymm1
	vmovapd ymm0, ymm7
	vmovapd ymm7, ymm6
	vmovapd xmm15, xmm8
	vmovapd xmm14, xmm9
	vmovapd ymm14, ymm9
	vmovapd ymm15, ymm11
	vmovapd xmm16, xmm22
	vmovapd xmm31, xmm26
	vmovapd ymm30, ymm18
	vmovapd ymm28, ymm17
	vmovapd xmm0{k4}, xmm2
	vmovapd ymm4{k1}, ymm4
	vmovapd zmm5{k6}, zmm0
	vmovapd xmm5{k7}, xmm4
	vmovapd ymm4{k1}, ymm6
	vmovapd zmm6{k6}, zmm6
	vmovapd oword [0xc80]{k6}, xmm5
	vmovapd yword [0x5de]{k5}, ymm7
	vmovapd zword [0x8b7]{k3}, zmm6
	vmovapd xmm5, oword [eax+1]
	vmovapd xmm3, oword [eax+64]
	vmovapd oword [eax+1], xmm0
	vmovapd oword [eax+64], xmm5
	vmovapd ymm2, yword [eax+1]
	vmovapd ymm1, yword [eax+64]
	vmovapd yword [eax+1], ymm3
	vmovapd yword [eax+64], ymm3
	vmovapd xmm5, oword [eax+1]
	vmovapd xmm7, oword [eax+64]
	vmovapd ymm0, yword [eax+1]
	vmovapd ymm3, yword [eax+64]
	vmovapd zmm6, zword [eax+1]
	vmovapd zmm6, zword [eax+64]
	vmovapd oword [eax+1], xmm0
	vmovapd oword [eax+64], xmm0
	vmovapd yword [eax+1], ymm0
	vmovapd yword [eax+64], ymm5
	vmovapd zword [eax+1], zmm7
	vmovapd zword [eax+64], zmm2
	vmovapd xmm6, [0x894]
	vmovapd [0x488], xmm2
	vmovapd ymm1, [0xf39]
	vmovapd [0x3aa], ymm1
	vmovapd xmm3, [0xfa0]
	vmovapd ymm1, [0xedf]
	vmovapd zmm1, [0xb1a]
	vmovapd [0x82b], xmm1
	vmovapd [0x80d], ymm6
	vmovapd [0xd53], zmm5
