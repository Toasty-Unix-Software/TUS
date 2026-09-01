	vendscalesh xmm4, xmm5, xmm4, 0xef
	vendscalesh xmm4, xmm4, 0xef
	vendscalesh xmm3, xmm0, xmm2, 0x74

%ifdef ERROR
	vendscalesh xmm10, xmm11, xmm15, 0x6a
	vendscalesh xmm19, xmm31, xmm25, 0x19
%endif
