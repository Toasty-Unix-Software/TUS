	vpshrdw xmm3, xmm7, xmm5, 0xba
	vpshrdw xmm3, xmm5, 0xba
	vpshrdw xmm7, xmm2, xmm3, 0x99
	vpshrdw ymm4, ymm5, ymm1, 0x41
	vpshrdw ymm4, ymm1, 0x41
	vpshrdw ymm1, ymm3, yword [0x167], 0x66
	vpshrdw zmm0, zmm0, zword [0xc2a], 0xd8
	vpshrdw zmm0, zword [0xc2a], 0xd8
	vpshrdw zmm5, zmm2, zmm6, 0x2a

%ifdef ERROR
	vpshrdw xmm10, xmm9, xmm12, 0x12
	vpshrdw ymm13, ymm13, ymm9, 0x63
	vpshrdw zmm14, zmm13, zmm9, 0xad
	vpshrdw xmm22, xmm19, xmm25, 0xf4
	vpshrdw ymm30, ymm28, ymm25, 0x4f
	vpshrdw zmm28, zmm21, zmm16, 0xe9
%endif
