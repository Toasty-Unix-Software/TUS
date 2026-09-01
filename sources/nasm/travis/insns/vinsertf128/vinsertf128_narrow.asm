	vinsertf128 ymm0, ymm4, oword [0x3b2], 0x4e
	vinsertf128 ymm0, oword [0x3b2], 0x4e
	vinsertf128 ymm4, ymm7, oword [0x9ef], 0x30

%ifdef ERROR
	vinsertf128 ymm9, ymm11, xmm8, 0x18
%endif
