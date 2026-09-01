	vpermilps xmm3, xmm4, oword [0xba1]
	vpermilps xmm3, oword [0xba1]
	vpermilps xmm1, xmm3, oword [0x39f]
	vpermilps ymm4, ymm1, yword [0x208]
	vpermilps ymm4, yword [0x208]
	vpermilps ymm1, ymm1, ymm6
	vpermilps xmm5, xmm4, 0xbd
	vpermilps xmm1, xmm0, 0x37
	vpermilps ymm7, ymm6, 0x5a
	vpermilps ymm4, ymm7, 0xa

%ifdef ERROR
	vpermilps xmm13, xmm13, xmm12
	vpermilps ymm14, ymm10, ymm11
	vpermilps xmm11, xmm11, 0x98
	vpermilps ymm12, ymm9, 0xa2
	vpermilps xmm23, xmm27, xmm25
	vpermilps ymm21, ymm30, ymm28
	vpermilps xmm19, xmm20, 0x1f
	vpermilps ymm25, ymm23, 0x47
%endif
