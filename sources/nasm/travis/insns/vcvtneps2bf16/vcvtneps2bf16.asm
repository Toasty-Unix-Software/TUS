default rel
	vcvtneps2bf16 xmm5, xmm3
	vcvtneps2bf16 xmm2, xmm0
	vcvtneps2bf16 xmm5, ymm5
	vcvtneps2bf16 xmm3, ymm1
	vcvtneps2bf16 xmm2, oword [0xcb7]
	vcvtneps2bf16 xmm1, oword [0xd3d]
	vcvtneps2bf16 xmm7, ymm1
	vcvtneps2bf16 xmm5, yword [0xef4]
	vcvtneps2bf16 xmm14, xmm10
	vcvtneps2bf16 xmm9, ymm11
	vcvtneps2bf16 xmm12, xmm13
	vcvtneps2bf16 xmm13, ymm12
	vcvtneps2bf16 xmm21, xmm23
	vcvtneps2bf16 xmm19, ymm22
	vcvtneps2bf16 xmm27, xmm18
	vcvtneps2bf16 xmm17, ymm17
	vcvtneps2bf16 xmm6{k6}, xmm7
	vcvtneps2bf16 xmm7{k6}, ymm0
	vcvtneps2bf16 ymm4{k6}, zword [0xc3b]
	vcvtneps2bf16 xmm3{k2}{z}, xmm1
	vcvtneps2bf16 xmm5{k7}{z}, yword [0x7e1]
	vcvtneps2bf16 ymm7{k4}{z}, zmm2
	vcvtneps2bf16 xmm0, oword [eax+1]
	vcvtneps2bf16 xmm2, oword [eax+64]
	vcvtneps2bf16 xmm5, yword [eax+1]
	vcvtneps2bf16 xmm1, yword [eax+64]
	vcvtneps2bf16 xmm3, oword [eax+1]
	vcvtneps2bf16 xmm3, oword [eax+64]
	vcvtneps2bf16 xmm0, yword [eax+1]
	vcvtneps2bf16 xmm3, yword [eax+64]
	vcvtneps2bf16 ymm4, zword [eax+1]
	vcvtneps2bf16 ymm3, zword [eax+64]
	vcvtneps2bf16 xmm3, [0x4e1]
	vcvtneps2bf16 xmm2, [0x9cd]
	vcvtneps2bf16 xmm3, [0x7b5]
	vcvtneps2bf16 xmm4, [0xf14]
	vcvtneps2bf16 ymm6, [0x783]
