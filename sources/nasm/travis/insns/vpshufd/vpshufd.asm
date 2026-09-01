default rel
	vpshufd xmm6, xmm5, 0x21
	vpshufd xmm5, xmm0, 0x5c
	vpshufd ymm3, ymm3, 0x76
	vpshufd ymm6, ymm1, 0x35
	vpshufd xmm6, xmm1, 0xc4
	vpshufd xmm3, oword [0x1f9], 0x39
	vpshufd ymm6, ymm7, 0x3f
	vpshufd ymm7, yword [0xad6], 0x6f
	vpshufd xmm14, xmm14, 0x18
	vpshufd ymm12, ymm15, 0x8e
	vpshufd xmm14, xmm15, 0x7c
	vpshufd ymm8, ymm11, 0x7d
	vpshufd xmm25, xmm30, 0x54
	vpshufd ymm28, ymm29, 0xc9
	vpshufd xmm16, xmm17, 0xf1
	vpshufd ymm22, ymm19, 0x99
	vpshufd xmm5{k6}, xmm2, 0xad
	vpshufd ymm7{k5}, yword [0x1fe], 0x9
	vpshufd zmm6{k5}, zmm4, 0x54
	vpshufd xmm3{k1}{z}, xmm2, 0xa8
	vpshufd ymm6{k2}{z}, yword [0x431], 0x1c
	vpshufd zmm0{k5}{z}, zmm7, 0x5c
	vpshufd xmm3, oword [eax+1], 0xf4
	vpshufd xmm7, oword [eax+64], 0x13
	vpshufd ymm3, yword [eax+1], 0x7c
	vpshufd ymm7, yword [eax+64], 0xcf
	vpshufd xmm0, oword [eax+1], 0xa8
	vpshufd xmm1, oword [eax+64], 0x67
	vpshufd ymm2, yword [eax+1], 0xe
	vpshufd ymm7, yword [eax+64], 0x39
	vpshufd zmm3, zword [eax+1], 0x9a
	vpshufd zmm2, zword [eax+64], 0x2f
	vpshufd xmm3, [0x59c], 0x97
	vpshufd ymm5, [0x18f], 0xd
	vpshufd xmm2, [0xd1e], 0x55
	vpshufd ymm5, [0x6a9], 0x5f
	vpshufd zmm1, [0x94a], 0x54
