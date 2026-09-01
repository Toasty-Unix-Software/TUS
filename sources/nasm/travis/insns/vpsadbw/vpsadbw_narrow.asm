	vpsadbw xmm3, xmm1, xmm1
	vpsadbw xmm3, xmm1
	vpsadbw xmm0, xmm4, oword [0x98a]
	vpsadbw ymm7, ymm2, ymm6
	vpsadbw ymm7, ymm6
	vpsadbw ymm5, ymm3, ymm2
	vpsadbw zmm2, zmm7, zword [0x6a1]
	vpsadbw zmm2, zword [0x6a1]
	vpsadbw zmm5, zmm5, zword [0x94d]

%ifdef ERROR
	vpsadbw xmm8, xmm13, xmm9
	vpsadbw ymm8, ymm13, ymm9
	vpsadbw zmm9, zmm8, zmm13
	vpsadbw xmm31, xmm30, xmm27
	vpsadbw ymm19, ymm17, ymm31
	vpsadbw zmm27, zmm19, zmm23
%endif
