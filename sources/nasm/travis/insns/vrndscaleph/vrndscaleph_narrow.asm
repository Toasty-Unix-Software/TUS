	vrndscaleph xmm2, oword [0x2b2], 0x74
	vrndscaleph xmm6, xmm1, 0x1f
	vrndscaleph ymm4, yword [0x937], 0xb8
	vrndscaleph ymm3, ymm1, 0xe6
	vrndscaleph zmm1, zmm0, 0x56
	vrndscaleph zmm4, zword [0x53c], 0x8

%ifdef ERROR
	vrndscaleph xmm10, xmm11, 0x48
	vrndscaleph ymm12, ymm11, 0x91
	vrndscaleph zmm13, zmm10, 0x97
	vrndscaleph xmm22, xmm29, 0x9a
	vrndscaleph ymm27, ymm27, 0x11
	vrndscaleph zmm16, zmm27, 0xd0
%endif
