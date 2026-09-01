default rel
	vpsllq xmm2, xmm7, xmm3
	vpsllq xmm2, xmm3
	vpsllq xmm3, xmm0, xmm5
	vpsllq xmm5, xmm1, 0xa0
	vpsllq xmm5, 0xa0
	vpsllq xmm3, xmm1, 0x13
	vpsllq ymm4, ymm4, oword [0xc7a]
	vpsllq ymm4, oword [0xc7a]
	vpsllq ymm3, ymm3, oword [0x185]
	vpsllq ymm2, ymm7, 0x1
	vpsllq ymm2, 0x1
	vpsllq ymm7, ymm4, 0xda
	vpsllq xmm14, xmm13, xmm14
	vpsllq xmm8, xmm14, 0xab
	vpsllq ymm8, ymm14, xmm11
	vpsllq ymm13, ymm10, 0x44
	vpsllq xmm27, xmm17, xmm21
	vpsllq xmm24, xmm28, 0x9f
	vpsllq ymm17, ymm19, xmm20
	vpsllq ymm24, ymm22, 0x81
	vpsllq xmm3{k1}, xmm7, oword [0xcff]
	vpsllq ymm5{k5}, ymm3, xmm2
	vpsllq zmm6{k3}, zmm5, xmm0
	vpsllq xmm7{k3}, xmm0, 0x6e
	vpsllq ymm2{k5}, ymm5, 0xda
	vpsllq zmm0{k5}, zmm1, 0xd9
	vpsllq xmm2{k5}{z}, xmm2, xmm4
	vpsllq ymm4{k7}{z}, ymm4, oword [0x7ff]
	vpsllq zmm5{k4}{z}, zmm0, xmm4
	vpsllq xmm5{k1}{z}, xmm5, 0x5c
	vpsllq ymm5{k2}{z}, yword [0xe95], 0x87
	vpsllq zmm5{k1}{z}, zmm5, 0x2d
	vpsllq xmm2, xmm2, oword [eax+1]
	vpsllq xmm6, xmm6, oword [eax+64]
	vpsllq ymm3, ymm1, oword [eax+1]
	vpsllq ymm0, ymm0, oword [eax+64]
	vpsllq xmm2, xmm1, oword [eax+1]
	vpsllq xmm2, xmm4, oword [eax+64]
	vpsllq ymm7, ymm0, oword [eax+1]
	vpsllq ymm6, ymm3, oword [eax+64]
	vpsllq zmm2, zmm6, oword [eax+1]
	vpsllq zmm1, zmm7, oword [eax+64]
	vpsllq xmm2, oword [eax+1], 0xf1
	vpsllq xmm1, oword [eax+64], 0xcc
	vpsllq ymm3, yword [eax+1], 0x46
	vpsllq ymm5, yword [eax+64], 0xdf
	vpsllq zmm0, zword [eax+1], 0xdf
	vpsllq zmm2, zword [eax+64], 0x4a
	vpsllq xmm1, xmm3, [0x88f]
	vpsllq ymm5, ymm4, [0x5bc]
	vpsllq xmm6, xmm6, [0x1c2]
	vpsllq ymm5, ymm5, [0x920]
	vpsllq zmm4, zmm2, [0x89c]
	vpsllq xmm7, [0x3a7], 0x92
	vpsllq ymm4, [0x952], 0x44
	vpsllq zmm2, [0x86f], 0xbb
