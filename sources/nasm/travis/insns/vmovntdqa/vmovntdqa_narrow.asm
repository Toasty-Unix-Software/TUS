	vmovntdqa xmm7, oword [0xc04]
	vmovntdqa xmm0, oword [0x6d6]
	vmovntdqa ymm7, yword [0x80b]
	vmovntdqa ymm6, yword [0x334]
	vmovntdqa zmm7, zword [0x6f5]
	vmovntdqa zmm7, zword [0xf74]

%ifdef ERROR
	vmovntdqa xmm8, oword [0x2f1]
	vmovntdqa ymm8, yword [0x564]
	vmovntdqa zmm14, zword [0x2a6]
	vmovntdqa xmm23, oword [0xc16]
	vmovntdqa ymm27, yword [0x94a]
	vmovntdqa zmm21, zword [0xdd1]
%endif
