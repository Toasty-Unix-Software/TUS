default rel
	vmaxbf16 xmm2, xmm5, oword [0x9bd]
	vmaxbf16 xmm4, xmm1, xmm2
	vmaxbf16 ymm2, ymm1, ymm7
	vmaxbf16 ymm7, ymm4, yword [0x76b]
	vmaxbf16 zmm6, zmm5, zmm2
	vmaxbf16 zmm2, zmm6, zword [0x2cc]
	vmaxbf16 xmm12, xmm12, xmm15
	vmaxbf16 ymm13, ymm8, ymm11
	vmaxbf16 zmm11, zmm10, zmm11
	vmaxbf16 xmm27, xmm18, xmm21
	vmaxbf16 ymm29, ymm23, ymm22
	vmaxbf16 zmm20, zmm21, zmm19
	vmaxbf16 xmm5{k2}, xmm0, xmm3
	vmaxbf16 ymm3{k6}, ymm7, ymm5
	vmaxbf16 zmm5{k5}, zmm1, zmm4
	vmaxbf16 xmm3{k5}{z}, xmm3, oword [0x5d1]
	vmaxbf16 ymm1{k4}{z}, ymm0, ymm1
	vmaxbf16 zmm2{k1}{z}, zmm7, zword [0xa3f]
	vmaxbf16 xmm4, xmm1, oword [eax+1]
	vmaxbf16 xmm1, xmm4, oword [eax+64]
	vmaxbf16 ymm6, ymm2, yword [eax+1]
	vmaxbf16 ymm2, ymm5, yword [eax+64]
	vmaxbf16 zmm7, zmm1, zword [eax+1]
	vmaxbf16 zmm6, zmm0, zword [eax+64]
	vmaxbf16 xmm7, xmm7, [0xd01]
	vmaxbf16 ymm2, ymm2, [0x66d]
	vmaxbf16 zmm1, zmm3, [0xf51]
