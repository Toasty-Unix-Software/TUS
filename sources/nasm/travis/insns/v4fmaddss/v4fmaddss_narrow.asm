	v4fmaddss xmm3, xmm6, [0xffc]
	v4fmaddss xmm2, xmm0, [0xc0a]

%ifdef ERROR
	v4fmaddss xmm8, xmm14, [0x4a7]
	v4fmaddss xmm17, xmm16, [0x3cc]
%endif
