	vscalefph xmm2, xmm3, oword [0xeb9]
	vscalefph xmm2, oword [0xeb9]
	vscalefph xmm6, xmm5, xmm5
	vscalefph ymm3, ymm0, ymm7
	vscalefph ymm3, ymm7
	vscalefph ymm0, ymm0, ymm2
	vscalefph zmm3, zmm7, zword [0x437]
	vscalefph zmm3, zword [0x437]
	vscalefph zmm5, zmm0, zword [0x84c]

%ifdef ERROR
	vscalefph xmm14, xmm14, xmm11
	vscalefph ymm11, ymm9, ymm13
	vscalefph zmm9, zmm15, zmm13
	vscalefph xmm21, xmm19, xmm23
	vscalefph ymm17, ymm29, ymm23
	vscalefph zmm23, zmm20, zmm17
%endif
