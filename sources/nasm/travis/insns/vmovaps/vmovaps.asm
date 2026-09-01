default rel
	vmovaps xmm3, xmm5
	vmovaps xmm5, xmm0
	vmovaps xmm0, xmm4
	vmovaps xmm5, xmm2
	vmovaps ymm6, ymm2
	vmovaps ymm6, yword [0x852]
	vmovaps yword [0xebc], ymm7
	vmovaps ymm4, ymm4
	vmovaps xmm14, xmm15
	vmovaps xmm15, xmm12
	vmovaps ymm14, ymm11
	vmovaps ymm12, ymm11
	vmovaps xmm21, xmm28
	vmovaps xmm24, xmm19
	vmovaps ymm29, ymm23
	vmovaps ymm26, ymm27
	vmovaps xmm4{k5}, xmm6
	vmovaps ymm7{k3}, yword [0xec6]
	vmovaps zmm2{k1}, zword [0x3a1]
	vmovaps xmm4{k3}, xmm2
	vmovaps ymm0{k4}, ymm5
	vmovaps zmm5{k7}, zmm5
	vmovaps oword [0x1f7]{k2}, xmm3
	vmovaps yword [0xef0]{k4}, ymm5
	vmovaps zword [0x33d]{k6}, zmm3
	vmovaps xmm6, oword [eax+1]
	vmovaps xmm4, oword [eax+64]
	vmovaps oword [eax+1], xmm7
	vmovaps oword [eax+64], xmm4
	vmovaps ymm2, yword [eax+1]
	vmovaps ymm6, yword [eax+64]
	vmovaps yword [eax+1], ymm0
	vmovaps yword [eax+64], ymm2
	vmovaps xmm3, oword [eax+1]
	vmovaps xmm6, oword [eax+64]
	vmovaps ymm4, yword [eax+1]
	vmovaps ymm4, yword [eax+64]
	vmovaps zmm1, zword [eax+1]
	vmovaps zmm6, zword [eax+64]
	vmovaps oword [eax+1], xmm3
	vmovaps oword [eax+64], xmm5
	vmovaps yword [eax+1], ymm3
	vmovaps yword [eax+64], ymm6
	vmovaps zword [eax+1], zmm7
	vmovaps zword [eax+64], zmm4
	vmovaps xmm2, [0x1b2]
	vmovaps [0x545], xmm5
	vmovaps ymm1, [0x5ef]
	vmovaps [0xeb1], ymm7
	vmovaps xmm0, [0x38e]
	vmovaps ymm4, [0x995]
	vmovaps zmm3, [0x509]
	vmovaps [0x316], xmm7
	vmovaps [0x835], ymm4
	vmovaps [0x6cc], zmm2
