default rel
	vmovdqu32 xmm4, oword [0x905]
	vmovdqu32 xmm7, xmm0
	vmovdqu32 ymm5, ymm4
	vmovdqu32 ymm6, yword [0xf26]
	vmovdqu32 zmm3, zword [0x21e]
	vmovdqu32 zmm0, zmm4
	vmovdqu32 xmm2, xmm4
	vmovdqu32 xmm3, xmm5
	vmovdqu32 xmm10, xmm12
	vmovdqu32 ymm15, ymm11
	vmovdqu32 zmm12, zmm14
	vmovdqu32 xmm12, xmm11
	vmovdqu32 xmm25, xmm19
	vmovdqu32 ymm18, ymm30
	vmovdqu32 zmm31, zmm31
	vmovdqu32 xmm17, xmm26
	vmovdqu32 xmm5{k1}, oword [0x552]
	vmovdqu32 ymm1{k7}, ymm5
	vmovdqu32 zmm5{k5}, zmm3
	vmovdqu32 xmm4{k6}, xmm3
	vmovdqu32 ymm3{k1}, ymm6
	vmovdqu32 zmm1{k4}, zmm2
	vmovdqu32 xmm1{k6}{z}, xmm6
	vmovdqu32 ymm6{k1}{z}, yword [0x7e3]
	vmovdqu32 zmm0{k7}{z}, zword [0x5f0]
	vmovdqu32 xmm5{k6}{z}, xmm4
	vmovdqu32 ymm3{k3}{z}, ymm3
	vmovdqu32 zword [0xfe6]{k1}{z}, zmm6
	vmovdqu32 xmm5, oword [eax+1]
	vmovdqu32 xmm3, oword [eax+64]
	vmovdqu32 ymm4, yword [eax+1]
	vmovdqu32 ymm3, yword [eax+64]
	vmovdqu32 zmm1, zword [eax+1]
	vmovdqu32 zmm2, zword [eax+64]
	vmovdqu32 oword [eax+1], xmm2
	vmovdqu32 oword [eax+64], xmm1
	vmovdqu32 yword [eax+1], ymm2
	vmovdqu32 yword [eax+64], ymm0
	vmovdqu32 zword [eax+1], zmm1
	vmovdqu32 zword [eax+64], zmm2
	vmovdqu32 xmm3, [0x97e]
	vmovdqu32 ymm3, [0x494]
	vmovdqu32 zmm2, [0x2af]
	vmovdqu32 [0x793], xmm2
	vmovdqu32 [0x480], ymm5
	vmovdqu32 [0x6f6], zmm6
