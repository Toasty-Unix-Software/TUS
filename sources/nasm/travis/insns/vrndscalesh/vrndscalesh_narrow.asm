	vrndscalesh xmm0, xmm7, xmm4, 0xa4
	vrndscalesh xmm0, xmm4, 0xa4
	vrndscalesh xmm3, xmm5, word [0x196], 0x6a

%ifdef ERROR
	vrndscalesh xmm13, xmm8, xmm13, 0x6a
	vrndscalesh xmm22, xmm25, xmm31, 0x6c
%endif
