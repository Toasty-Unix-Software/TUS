	vrsqrtps xmm1, xmm5
	vrsqrtps xmm3, oword [0xbf7]
	vrsqrtps ymm7, yword [0xa63]
	vrsqrtps ymm2, yword [0x8fd]

%ifdef ERROR
	vrsqrtps xmm8, xmm8
	vrsqrtps ymm8, ymm9
%endif
