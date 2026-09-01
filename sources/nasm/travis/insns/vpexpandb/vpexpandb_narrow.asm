	vpexpandb xmm0, oword [0x8fc]
	vpexpandb xmm4, oword [0x222]
	vpexpandb ymm0, yword [0x292]
	vpexpandb ymm0, yword [0xdcc]
	vpexpandb zmm7, zmm3
	vpexpandb zmm4, zword [0xbcb]

%ifdef ERROR
	vpexpandb xmm10, xmm12
	vpexpandb ymm13, ymm11
	vpexpandb zmm10, zmm9
	vpexpandb xmm31, xmm23
	vpexpandb ymm24, ymm30
	vpexpandb zmm23, zmm22
%endif
