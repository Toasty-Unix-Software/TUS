default rel
	vpopcntb xmm7, xmm4
	vpopcntb xmm7, oword [0xc6d]
	vpopcntb ymm5, ymm7
	vpopcntb ymm4, ymm4
	vpopcntb zmm1, zmm7
	vpopcntb zmm5, zmm2
	vpopcntb xmm15, xmm14
	vpopcntb ymm14, ymm14
	vpopcntb zmm13, zmm14
	vpopcntb xmm20, xmm29
	vpopcntb ymm25, ymm18
	vpopcntb zmm22, zmm26
	vpopcntb xmm2{k7}, xmm4
	vpopcntb ymm0{k5}, ymm3
	vpopcntb zmm7{k1}, zword [0x2d2]
	vpopcntb xmm3{k7}{z}, oword [0x603]
	vpopcntb ymm4{k1}{z}, yword [0xdd7]
	vpopcntb zmm5{k2}{z}, zmm3
	vpopcntb xmm5, oword [eax+1]
	vpopcntb xmm3, oword [eax+64]
	vpopcntb ymm1, yword [eax+1]
	vpopcntb ymm4, yword [eax+64]
	vpopcntb zmm7, zword [eax+1]
	vpopcntb zmm0, zword [eax+64]
	vpopcntb xmm0, [0x6e2]
	vpopcntb ymm0, [0xfcb]
	vpopcntb zmm3, [0xacf]
