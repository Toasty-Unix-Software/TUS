	vroundps xmm3, xmm2, 0x94
	vroundps xmm1, xmm4, 0xbe
	vroundps ymm3, yword [0x109], 0x38
	vroundps ymm4, ymm4, 0xf7

%ifdef ERROR
	vroundps xmm12, xmm8, 0xc4
	vroundps ymm12, ymm15, 0xe3
%endif
