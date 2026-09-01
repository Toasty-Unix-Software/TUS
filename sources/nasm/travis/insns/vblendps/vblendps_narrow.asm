	vblendps xmm4, xmm4, oword [0x99f], 0x25
	vblendps xmm4, oword [0x99f], 0x25
	vblendps xmm7, xmm0, xmm4, 0x7
	vblendps ymm2, ymm2, ymm7, 0x4b
	vblendps ymm2, ymm7, 0x4b
	vblendps ymm7, ymm2, yword [0xcfe], 0x42

%ifdef ERROR
	vblendps xmm12, xmm12, xmm10, 0x5d
	vblendps ymm13, ymm9, ymm12, 0x4a
%endif
