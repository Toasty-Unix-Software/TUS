default rel
	vfrczps xmm7, xmm6
	vfrczps xmm7
	vfrczps xmm0, xmm1
	vfrczps ymm5, yword [0x9af]
	vfrczps ymm5
	vfrczps ymm3, yword [0xf36]
	vfrczps xmm14, xmm8
	vfrczps ymm14, ymm9
	vfrczps xmm7, oword [eax+1]
	vfrczps xmm5, oword [eax+64]
	vfrczps ymm3, yword [eax+1]
	vfrczps ymm7, yword [eax+64]
	vfrczps xmm5, [0xc53]
	vfrczps ymm3, [0x951]
