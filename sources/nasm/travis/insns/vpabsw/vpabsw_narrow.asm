	vpabsw xmm1, oword [0xe00]
	vpabsw xmm3, xmm3
	vpabsw ymm7, ymm5
	vpabsw ymm0, yword [0xddf]
	vpabsw xmm7, xmm4
	vpabsw xmm5, oword [0x45f]
	vpabsw ymm3, yword [0xa3c]
	vpabsw ymm7, ymm5

%ifdef ERROR
	vpabsw xmm9, xmm8
	vpabsw ymm12, ymm10
	vpabsw xmm12, xmm8
	vpabsw ymm8, ymm15
	vpabsw xmm20, xmm28
	vpabsw ymm24, ymm24
	vpabsw xmm24, xmm28
	vpabsw ymm31, ymm29
%endif
