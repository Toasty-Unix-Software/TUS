	v4fmaddps zmm3, zmm5, [0xe02]
	v4fmaddps zmm5, zmm4, [0x78e]

%ifdef ERROR
	v4fmaddps zmm13, zmm14, [0xef2]
	v4fmaddps zmm24, zmm25, [0xb52]
%endif
