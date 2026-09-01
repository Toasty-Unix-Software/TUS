	vmulsh xmm3, xmm0, xmm1
	vmulsh xmm3, xmm1
	vmulsh xmm4, xmm7, word [0xa91]

%ifdef ERROR
	vmulsh xmm9, xmm14, xmm8
	vmulsh xmm23, xmm22, xmm17
%endif
