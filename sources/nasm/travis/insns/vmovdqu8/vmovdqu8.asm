default rel
	vmovdqu8 xmm5, oword [0x847]
	vmovdqu8 xmm0, xmm5
	vmovdqu8 ymm1, ymm7
	vmovdqu8 ymm0, ymm5
	vmovdqu8 zmm4, zmm4
	vmovdqu8 zmm7, zmm1
	vmovdqu8 xmm0, xmm1
	vmovdqu8 xmm2, xmm6
	vmovdqu8 xmm9, xmm11
	vmovdqu8 ymm15, ymm14
	vmovdqu8 zmm15, zmm9
	vmovdqu8 xmm15, xmm10
	vmovdqu8 xmm29, xmm16
	vmovdqu8 ymm22, ymm20
	vmovdqu8 zmm21, zmm16
	vmovdqu8 xmm16, xmm26
	vmovdqu8 xmm0{k2}, xmm6
	vmovdqu8 ymm0{k3}, ymm7
	vmovdqu8 zmm5{k3}, zmm7
	vmovdqu8 xmm6{k6}, xmm3
	vmovdqu8 yword [0x213]{k6}, ymm0
	vmovdqu8 zmm6{k2}, zmm5
	vmovdqu8 xmm3{k7}{z}, xmm2
	vmovdqu8 ymm3{k7}{z}, ymm7
	vmovdqu8 zmm1{k7}{z}, zmm3
	vmovdqu8 xmm7{k2}{z}, xmm1
	vmovdqu8 yword [0x437]{k7}{z}, ymm3
	vmovdqu8 zmm0{k6}{z}, zmm7
	vmovdqu8 xmm6, oword [eax+1]
	vmovdqu8 xmm7, oword [eax+64]
	vmovdqu8 ymm4, yword [eax+1]
	vmovdqu8 ymm0, yword [eax+64]
	vmovdqu8 zmm5, zword [eax+1]
	vmovdqu8 zmm5, zword [eax+64]
	vmovdqu8 oword [eax+1], xmm3
	vmovdqu8 oword [eax+64], xmm5
	vmovdqu8 yword [eax+1], ymm0
	vmovdqu8 yword [eax+64], ymm6
	vmovdqu8 zword [eax+1], zmm2
	vmovdqu8 zword [eax+64], zmm6
	vmovdqu8 xmm1, [0x630]
	vmovdqu8 ymm1, [0x50a]
	vmovdqu8 zmm6, [0x9fd]
	vmovdqu8 [0x35d], xmm5
	vmovdqu8 [0x2f4], ymm1
	vmovdqu8 [0x84c], zmm3
