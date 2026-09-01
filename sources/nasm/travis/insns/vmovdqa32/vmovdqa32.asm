default rel
	vmovdqa32 xmm6, xmm6
	vmovdqa32 xmm7, xmm6
	vmovdqa32 ymm0, ymm4
	vmovdqa32 ymm4, yword [0xb90]
	vmovdqa32 zmm0, zmm6
	vmovdqa32 zmm1, zmm0
	vmovdqa32 oword [0xc5e], xmm6
	vmovdqa32 xmm2, xmm3
	vmovdqa32 xmm13, xmm10
	vmovdqa32 ymm10, ymm10
	vmovdqa32 zmm9, zmm15
	vmovdqa32 xmm9, xmm14
	vmovdqa32 xmm20, xmm25
	vmovdqa32 ymm21, ymm17
	vmovdqa32 zmm22, zmm18
	vmovdqa32 xmm16, xmm26
	vmovdqa32 xmm2{k3}, xmm2
	vmovdqa32 ymm0{k3}, ymm5
	vmovdqa32 zmm0{k7}, zmm6
	vmovdqa32 oword [0x8eb]{k3}, xmm2
	vmovdqa32 ymm1{k1}, ymm6
	vmovdqa32 zmm5{k1}, zmm7
	vmovdqa32 xmm1{k2}{z}, oword [0x82d]
	vmovdqa32 ymm3{k3}{z}, ymm2
	vmovdqa32 zmm6{k7}{z}, zmm4
	vmovdqa32 oword [0x3ba]{k2}{z}, xmm1
	vmovdqa32 ymm2{k4}{z}, ymm5
	vmovdqa32 zmm4{k4}{z}, zmm4
	vmovdqa32 xmm3, oword [eax+1]
	vmovdqa32 xmm5, oword [eax+64]
	vmovdqa32 ymm1, yword [eax+1]
	vmovdqa32 ymm5, yword [eax+64]
	vmovdqa32 zmm4, zword [eax+1]
	vmovdqa32 zmm2, zword [eax+64]
	vmovdqa32 oword [eax+1], xmm5
	vmovdqa32 oword [eax+64], xmm1
	vmovdqa32 yword [eax+1], ymm2
	vmovdqa32 yword [eax+64], ymm0
	vmovdqa32 zword [eax+1], zmm0
	vmovdqa32 zword [eax+64], zmm2
	vmovdqa32 xmm5, [0x4e8]
	vmovdqa32 ymm4, [0x2f6]
	vmovdqa32 zmm3, [0xb35]
	vmovdqa32 [0xe2f], xmm1
	vmovdqa32 [0x2e4], ymm3
	vmovdqa32 [0x95c], zmm3
