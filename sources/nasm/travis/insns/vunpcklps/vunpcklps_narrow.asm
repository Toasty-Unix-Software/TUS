	vunpcklps xmm3, xmm2, xmm6
	vunpcklps xmm3, xmm6
	vunpcklps xmm5, xmm6, xmm4
	vunpcklps ymm0, ymm5, yword [0x5f3]
	vunpcklps ymm0, yword [0x5f3]
	vunpcklps ymm1, ymm4, ymm5
	vunpcklps xmm3, xmm5, xmm3
	vunpcklps xmm3, xmm3
	vunpcklps xmm1, xmm5, xmm5
	vunpcklps ymm3, ymm5, yword [0x3fc]
	vunpcklps ymm3, yword [0x3fc]
	vunpcklps ymm7, ymm3, yword [0xe8c]

%ifdef ERROR
	vunpcklps xmm12, xmm15, xmm11
	vunpcklps ymm12, ymm9, ymm9
	vunpcklps xmm12, xmm13, xmm11
	vunpcklps ymm13, ymm14, ymm11
	vunpcklps xmm26, xmm19, xmm16
	vunpcklps ymm30, ymm31, ymm22
	vunpcklps xmm16, xmm28, xmm18
	vunpcklps ymm21, ymm21, ymm21
%endif
