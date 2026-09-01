	vrcpph xmm3, oword [0xfb1]
	vrcpph xmm3, xmm4
	vrcpph ymm2, ymm7
	vrcpph ymm7, yword [0xfd9]
	vrcpph zmm2, zword [0xa11]
	vrcpph zmm1, zword [0x62b]

%ifdef ERROR
	vrcpph xmm8, xmm10
	vrcpph ymm11, ymm8
	vrcpph zmm9, zmm13
	vrcpph xmm24, xmm24
	vrcpph ymm29, ymm22
	vrcpph zmm16, zmm26
%endif
