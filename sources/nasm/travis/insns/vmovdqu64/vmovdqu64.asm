default rel
	vmovdqu64 xmm7, xmm2
	vmovdqu64 xmm4, xmm3
	vmovdqu64 ymm7, yword [0x251]
	vmovdqu64 ymm2, ymm5
	vmovdqu64 zmm0, zmm6
	vmovdqu64 zmm2, zmm6
	vmovdqu64 oword [0xecd], xmm0
	vmovdqu64 xmm6, xmm5
	vmovdqu64 xmm8, xmm11
	vmovdqu64 ymm12, ymm10
	vmovdqu64 zmm12, zmm12
	vmovdqu64 xmm13, xmm12
	vmovdqu64 xmm27, xmm26
	vmovdqu64 ymm21, ymm25
	vmovdqu64 zmm31, zmm28
	vmovdqu64 xmm19, xmm20
	vmovdqu64 xmm7{k3}, xmm0
	vmovdqu64 ymm0{k2}, ymm7
	vmovdqu64 zmm3{k2}, zword [0x5a4]
	vmovdqu64 oword [0xd17]{k2}, xmm6
	vmovdqu64 ymm4{k6}, ymm3
	vmovdqu64 zword [0x6bd]{k7}, zmm7
	vmovdqu64 xmm1{k6}{z}, oword [0xff1]
	vmovdqu64 ymm7{k5}{z}, ymm6
	vmovdqu64 zmm7{k4}{z}, zword [0x7d4]
	vmovdqu64 xmm6{k4}{z}, xmm6
	vmovdqu64 ymm7{k5}{z}, ymm7
	vmovdqu64 zword [0xef0]{k5}{z}, zmm3
	vmovdqu64 xmm6, oword [eax+1]
	vmovdqu64 xmm1, oword [eax+64]
	vmovdqu64 ymm1, yword [eax+1]
	vmovdqu64 ymm2, yword [eax+64]
	vmovdqu64 zmm0, zword [eax+1]
	vmovdqu64 zmm0, zword [eax+64]
	vmovdqu64 oword [eax+1], xmm3
	vmovdqu64 oword [eax+64], xmm6
	vmovdqu64 yword [eax+1], ymm1
	vmovdqu64 yword [eax+64], ymm0
	vmovdqu64 zword [eax+1], zmm1
	vmovdqu64 zword [eax+64], zmm4
	vmovdqu64 xmm6, [0x18e]
	vmovdqu64 ymm4, [0x257]
	vmovdqu64 zmm0, [0x6dc]
	vmovdqu64 [0x7ef], xmm7
	vmovdqu64 [0x2e4], ymm4
	vmovdqu64 [0xaeb], zmm7
