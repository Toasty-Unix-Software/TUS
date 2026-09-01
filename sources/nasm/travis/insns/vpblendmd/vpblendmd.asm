default rel
	vpblendmd xmm7, xmm0, xmm7
	vpblendmd xmm1, xmm3, oword [0xa99]
	vpblendmd ymm6, ymm3, yword [0x2b2]
	vpblendmd ymm5, ymm2, ymm6
	vpblendmd zmm7, zmm6, zmm6
	vpblendmd zmm5, zmm7, zword [0x4ed]
	vpblendmd xmm10, xmm11, xmm13
	vpblendmd ymm15, ymm14, ymm10
	vpblendmd zmm15, zmm12, zmm15
	vpblendmd xmm20, xmm22, xmm20
	vpblendmd ymm18, ymm16, ymm29
	vpblendmd zmm25, zmm30, zmm21
	vpblendmd xmm6{k3}, xmm5, xmm5
	vpblendmd ymm6{k1}, ymm3, ymm4
	vpblendmd zmm3{k3}, zmm7, zmm1
	vpblendmd xmm7{k2}{z}, xmm5, xmm0
	vpblendmd ymm4{k3}{z}, ymm6, ymm0
	vpblendmd zmm4{k6}{z}, zmm3, zword [0x96f]
	vpblendmd xmm0, xmm0, oword [eax+1]
	vpblendmd xmm3, xmm6, oword [eax+64]
	vpblendmd ymm6, ymm0, yword [eax+1]
	vpblendmd ymm5, ymm7, yword [eax+64]
	vpblendmd zmm3, zmm0, zword [eax+1]
	vpblendmd zmm1, zmm0, zword [eax+64]
	vpblendmd xmm2, xmm7, [0xf7e]
	vpblendmd ymm0, ymm1, [0xdbb]
	vpblendmd zmm0, zmm5, [0x55a]
