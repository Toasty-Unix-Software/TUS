default rel
	vmovdqu16 xmm4, oword [0xa70]
	vmovdqu16 xmm5, xmm6
	vmovdqu16 ymm7, ymm3
	vmovdqu16 ymm4, ymm2
	vmovdqu16 zmm6, zword [0xd0c]
	vmovdqu16 zmm2, zmm3
	vmovdqu16 xmm5, xmm0
	vmovdqu16 oword [0xc8b], xmm4
	vmovdqu16 xmm8, xmm9
	vmovdqu16 ymm15, ymm14
	vmovdqu16 zmm15, zmm13
	vmovdqu16 xmm14, xmm9
	vmovdqu16 xmm19, xmm18
	vmovdqu16 ymm16, ymm27
	vmovdqu16 zmm25, zmm18
	vmovdqu16 xmm28, xmm29
	vmovdqu16 xmm4{k5}, oword [0xf5a]
	vmovdqu16 ymm7{k3}, ymm6
	vmovdqu16 zmm1{k1}, zmm3
	vmovdqu16 xmm1{k3}, xmm2
	vmovdqu16 ymm2{k6}, ymm2
	vmovdqu16 zmm0{k2}, zmm0
	vmovdqu16 xmm7{k2}{z}, xmm4
	vmovdqu16 ymm2{k2}{z}, ymm7
	vmovdqu16 zmm2{k3}{z}, zmm6
	vmovdqu16 oword [0xfc0]{k1}{z}, xmm1
	vmovdqu16 ymm7{k4}{z}, ymm6
	vmovdqu16 zmm6{k7}{z}, zmm5
	vmovdqu16 xmm7, oword [eax+1]
	vmovdqu16 xmm2, oword [eax+64]
	vmovdqu16 ymm2, yword [eax+1]
	vmovdqu16 ymm5, yword [eax+64]
	vmovdqu16 zmm4, zword [eax+1]
	vmovdqu16 zmm2, zword [eax+64]
	vmovdqu16 oword [eax+1], xmm0
	vmovdqu16 oword [eax+64], xmm5
	vmovdqu16 yword [eax+1], ymm3
	vmovdqu16 yword [eax+64], ymm5
	vmovdqu16 zword [eax+1], zmm2
	vmovdqu16 zword [eax+64], zmm4
	vmovdqu16 xmm5, [0x7c7]
	vmovdqu16 ymm6, [0x140]
	vmovdqu16 zmm2, [0xd62]
	vmovdqu16 [0x920], xmm1
	vmovdqu16 [0x78b], ymm3
	vmovdqu16 [0xfc8], zmm4
