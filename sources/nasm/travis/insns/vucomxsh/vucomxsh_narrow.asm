	vucomxsh xmm5, xmm0
	vucomxsh xmm7, word [0x4c1]

%ifdef ERROR
	vucomxsh xmm12, xmm10
	vucomxsh xmm29, xmm31
%endif
