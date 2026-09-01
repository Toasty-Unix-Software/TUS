	vcvtneps2bf16 xmm5, xmm3
	vcvtneps2bf16 xmm2, xmm0
	vcvtneps2bf16 xmm5, ymm5
	vcvtneps2bf16 xmm3, ymm1
	vcvtneps2bf16 xmm2, oword [0xcb7]
	vcvtneps2bf16 xmm1, oword [0xd3d]
	vcvtneps2bf16 xmm7, ymm1
	vcvtneps2bf16 xmm5, yword [0xef4]

%ifdef ERROR
	vcvtneps2bf16 xmm14, xmm10
	vcvtneps2bf16 xmm9, ymm11
	vcvtneps2bf16 xmm12, xmm13
	vcvtneps2bf16 xmm13, ymm12
	vcvtneps2bf16 xmm21, xmm23
	vcvtneps2bf16 xmm19, ymm22
	vcvtneps2bf16 xmm27, xmm18
	vcvtneps2bf16 xmm17, ymm17
%endif
