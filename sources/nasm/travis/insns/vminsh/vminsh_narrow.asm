	vminsh xmm1, xmm7, xmm6
	vminsh xmm4, xmm5, xmm6
	vminsh xmm4, xmm1, word [0xb7a]
	vminsh xmm4, word [0xb7a]
	vminsh xmm4, xmm7, xmm1

%ifdef ERROR
	vminsh xmm13, xmm14, xmm9
	vminsh xmm10, xmm13, xmm10
	vminsh xmm28, xmm25, xmm26
	vminsh xmm28, xmm27, xmm19
%endif
