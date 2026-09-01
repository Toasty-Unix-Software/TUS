	vpexpandd xmm1, oword [0xe3c]
	vpexpandd xmm0, oword [0x72a]
	vpexpandd ymm6, yword [0xb05]
	vpexpandd ymm6, yword [0x807]
	vpexpandd zmm5, zword [0xbfb]
	vpexpandd zmm1, zword [0x557]
	vpexpandd xmm5, xmm2
	vpexpandd xmm5, xmm3

%ifdef ERROR
	vpexpandd xmm13, oword [0x95d]
	vpexpandd ymm8, yword [0xa9a]
	vpexpandd zmm13, zword [0xbb3]
	vpexpandd xmm14, xmm12
	vpexpandd xmm18, oword [0x6f2]
	vpexpandd ymm26, yword [0xdbd]
	vpexpandd zmm27, zword [0xadc]
	vpexpandd xmm20, xmm28
%endif
