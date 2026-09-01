	vfmaddcsh xmm7, xmm7, xmm4
	vfmaddcsh xmm7, xmm4
	vfmaddcsh xmm0, xmm4, xmm2

%ifdef ERROR
	vfmaddcsh xmm8, xmm14, xmm9
	vfmaddcsh xmm17, xmm29, xmm25
%endif
