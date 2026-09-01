	vfrczps xmm7, xmm6
	vfrczps xmm7
	vfrczps xmm0, xmm1
	vfrczps ymm5, yword [0x9af]
	vfrczps ymm5
	vfrczps ymm3, yword [0xf36]

%ifdef ERROR
	vfrczps xmm14, xmm8
	vfrczps ymm14, ymm9
%endif
