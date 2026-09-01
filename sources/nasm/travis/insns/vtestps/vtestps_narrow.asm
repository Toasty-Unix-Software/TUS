	vtestps xmm6, xmm1
	vtestps xmm4, xmm7
	vtestps ymm6, yword [0xe8b]
	vtestps ymm0, yword [0x60c]

%ifdef ERROR
	vtestps xmm8, xmm10
	vtestps ymm9, ymm12
%endif
