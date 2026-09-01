default rel
	vpsadbw xmm3, xmm1, xmm1
	vpsadbw xmm3, xmm1
	vpsadbw xmm0, xmm4, oword [0x98a]
	vpsadbw ymm7, ymm2, ymm6
	vpsadbw ymm7, ymm6
	vpsadbw ymm5, ymm3, ymm2
	vpsadbw zmm2, zmm7, zword [0x6a1]
	vpsadbw zmm2, zword [0x6a1]
	vpsadbw zmm5, zmm5, zword [0x94d]
	vpsadbw xmm8, xmm13, xmm9
	vpsadbw ymm8, ymm13, ymm9
	vpsadbw zmm9, zmm8, zmm13
	vpsadbw xmm31, xmm30, xmm27
	vpsadbw ymm19, ymm17, ymm31
	vpsadbw zmm27, zmm19, zmm23
	vpsadbw xmm0, xmm7, oword [eax+1]
	vpsadbw xmm1, xmm3, oword [eax+64]
	vpsadbw ymm6, ymm1, yword [eax+1]
	vpsadbw ymm1, ymm4, yword [eax+64]
	vpsadbw zmm5, zmm4, zword [eax+1]
	vpsadbw zmm0, zmm0, zword [eax+64]
	vpsadbw xmm3, xmm2, [0xc2a]
	vpsadbw ymm4, ymm2, [0x856]
	vpsadbw zmm3, zmm3, [0xd52]
