default rel
	vprold xmm2, oword [0xbd4], 0x7d
	vprold xmm2, 0x7d
	vprold xmm0, xmm5, 0xb6
	vprold ymm6, ymm0, 0xf
	vprold ymm6, 0xf
	vprold ymm5, yword [0xd9d], 0xb7
	vprold zmm3, zword [0x26b], 0x85
	vprold zmm3, 0x85
	vprold zmm5, zmm7, 0xae
	vprold xmm8, xmm14, 0x45
	vprold ymm8, ymm12, 0xfa
	vprold zmm12, zmm14, 0xcc
	vprold xmm26, xmm18, 0xf7
	vprold ymm24, ymm17, 0x18
	vprold zmm31, zmm26, 0x20
	vprold xmm3{k7}, xmm7, 0x1f
	vprold ymm5{k3}, ymm3, 0xd5
	vprold zmm0{k1}, zword [0x475], 0xc
	vprold xmm0{k4}{z}, oword [0x331], 0x3e
	vprold ymm6{k7}{z}, ymm6, 0x5c
	vprold zmm7{k2}{z}, zmm6, 0x4c
	vprold xmm3, oword [eax+1], 0xf8
	vprold xmm7, oword [eax+64], 0x88
	vprold ymm4, yword [eax+1], 0x37
	vprold ymm5, yword [eax+64], 0x96
	vprold zmm2, zword [eax+1], 0xa1
	vprold zmm2, zword [eax+64], 0xb3
	vprold xmm1, [0x6c9], 0x3d
	vprold ymm1, [0x680], 0x17
	vprold zmm6, [0x1dc], 0x4b
