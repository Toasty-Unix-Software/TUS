	vreducesh xmm2, xmm4, xmm6, 0xc0
	vreducesh xmm2, xmm6, 0xc0
	vreducesh xmm3, xmm4, xmm0, 0xaf

%ifdef ERROR
	vreducesh xmm8, xmm9, xmm14, 0x68
	vreducesh xmm23, xmm25, xmm22, 0x52
%endif
