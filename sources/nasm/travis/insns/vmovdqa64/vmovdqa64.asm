default rel
	vmovdqa64 xmm0, oword [0x2ed]
	vmovdqa64 xmm4, xmm3
	vmovdqa64 ymm0, yword [0xc2e]
	vmovdqa64 ymm4, ymm2
	vmovdqa64 zmm0, zmm2
	vmovdqa64 zmm2, zword [0x72f]
	vmovdqa64 xmm6, xmm3
	vmovdqa64 xmm3, xmm5
	vmovdqa64 xmm8, xmm11
	vmovdqa64 ymm8, ymm8
	vmovdqa64 zmm11, zmm15
	vmovdqa64 xmm8, xmm12
	vmovdqa64 xmm31, xmm17
	vmovdqa64 ymm27, ymm24
	vmovdqa64 zmm27, zmm21
	vmovdqa64 xmm31, xmm18
	vmovdqa64 xmm7{k3}, xmm1
	vmovdqa64 ymm1{k3}, ymm1
	vmovdqa64 zmm3{k6}, zmm1
	vmovdqa64 xmm0{k2}, xmm0
	vmovdqa64 ymm2{k6}, ymm0
	vmovdqa64 zmm5{k1}, zmm2
	vmovdqa64 xmm4{k1}{z}, xmm6
	vmovdqa64 ymm0{k4}{z}, ymm3
	vmovdqa64 zmm2{k7}{z}, zmm2
	vmovdqa64 xmm2{k3}{z}, xmm3
	vmovdqa64 yword [0x11e]{k1}{z}, ymm4
	vmovdqa64 zmm2{k5}{z}, zmm3
	vmovdqa64 xmm6, oword [eax+1]
	vmovdqa64 xmm4, oword [eax+64]
	vmovdqa64 ymm2, yword [eax+1]
	vmovdqa64 ymm2, yword [eax+64]
	vmovdqa64 zmm0, zword [eax+1]
	vmovdqa64 zmm2, zword [eax+64]
	vmovdqa64 oword [eax+1], xmm5
	vmovdqa64 oword [eax+64], xmm4
	vmovdqa64 yword [eax+1], ymm1
	vmovdqa64 yword [eax+64], ymm3
	vmovdqa64 zword [eax+1], zmm5
	vmovdqa64 zword [eax+64], zmm0
	vmovdqa64 xmm1, [0x2a9]
	vmovdqa64 ymm4, [0xa95]
	vmovdqa64 zmm3, [0xa6e]
	vmovdqa64 [0x90d], xmm3
	vmovdqa64 [0x5c3], ymm4
	vmovdqa64 [0x4cb], zmm0
