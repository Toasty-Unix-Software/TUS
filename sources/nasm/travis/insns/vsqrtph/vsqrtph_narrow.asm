	vsqrtph xmm3, oword [0xa87]
	vsqrtph xmm0, oword [0x4a5]
	vsqrtph ymm5, yword [0x253]
	vsqrtph ymm5, ymm5
	vsqrtph zmm7, zmm3
	vsqrtph zmm5, zmm5

%ifdef ERROR
	vsqrtph xmm14, xmm10
	vsqrtph ymm15, ymm13
	vsqrtph zmm14, zmm12
	vsqrtph xmm17, xmm31
	vsqrtph ymm22, ymm17
	vsqrtph zmm21, zmm29
%endif
