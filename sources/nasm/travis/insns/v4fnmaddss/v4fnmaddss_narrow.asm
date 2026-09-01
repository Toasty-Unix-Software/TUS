	v4fnmaddss xmm0, xmm1, [0x81d]
	v4fnmaddss xmm2, xmm2, [0xfbe]

%ifdef ERROR
	v4fnmaddss xmm11, xmm9, [0x55f]
	v4fnmaddss xmm31, xmm22, [0x86e]
%endif
