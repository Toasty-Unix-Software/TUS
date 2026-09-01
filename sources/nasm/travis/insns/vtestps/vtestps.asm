default rel
	vtestps xmm6, xmm1
	vtestps xmm4, xmm7
	vtestps ymm6, yword [0xe8b]
	vtestps ymm0, yword [0x60c]
	vtestps xmm8, xmm10
	vtestps ymm9, ymm12
	vtestps xmm0, oword [eax+1]
	vtestps xmm0, oword [eax+64]
	vtestps ymm0, yword [eax+1]
	vtestps ymm7, yword [eax+64]
	vtestps xmm6, [0xb0f]
	vtestps ymm5, [0x1e7]
