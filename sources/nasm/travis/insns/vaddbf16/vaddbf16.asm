default rel
	vaddbf16 xmm7, xmm1, oword [0x4fd]
	vaddbf16 xmm4, xmm4, oword [0xe03]
	vaddbf16 ymm7, ymm2, yword [0x779]
	vaddbf16 ymm1, ymm6, ymm7
	vaddbf16 zmm1, zmm3, zmm3
	vaddbf16 zmm2, zmm3, zmm0
	vaddbf16 xmm11, xmm12, xmm8
	vaddbf16 ymm11, ymm12, ymm12
	vaddbf16 zmm15, zmm9, zmm10
	vaddbf16 xmm24, xmm19, xmm24
	vaddbf16 ymm18, ymm29, ymm20
	vaddbf16 zmm17, zmm25, zmm27
	vaddbf16 xmm5{k1}, xmm6, oword [0x9d7]
	vaddbf16 ymm4{k3}, ymm3, yword [0xb92]
	vaddbf16 zmm2{k1}, zmm0, zmm3
	vaddbf16 xmm3{k5}{z}, xmm5, xmm3
	vaddbf16 ymm3{k5}{z}, ymm7, ymm0
	vaddbf16 zmm2{k1}{z}, zmm2, zword [0x39d]
	vaddbf16 xmm0, xmm0, oword [eax+1]
	vaddbf16 xmm1, xmm2, oword [eax+64]
	vaddbf16 ymm5, ymm0, yword [eax+1]
	vaddbf16 ymm0, ymm4, yword [eax+64]
	vaddbf16 zmm5, zmm6, zword [eax+1]
	vaddbf16 zmm5, zmm2, zword [eax+64]
	vaddbf16 xmm5, xmm6, [0x6e1]
	vaddbf16 ymm0, ymm3, [0x7dc]
	vaddbf16 zmm6, zmm2, [0x4e4]
