	vpsubsw xmm1, xmm5, xmm5
	vpsubsw xmm1, xmm5
	vpsubsw xmm4, xmm0, xmm5
	vpsubsw ymm3, ymm1, yword [0x14a]
	vpsubsw ymm3, yword [0x14a]
	vpsubsw ymm3, ymm7, ymm7
	vpsubsw xmm7, xmm0, xmm5
	vpsubsw xmm7, xmm5
	vpsubsw xmm5, xmm6, xmm4
	vpsubsw ymm6, ymm4, ymm6
	vpsubsw ymm6, ymm6
	vpsubsw ymm1, ymm1, ymm7

%ifdef ERROR
	vpsubsw xmm10, xmm8, xmm13
	vpsubsw ymm8, ymm14, ymm9
	vpsubsw xmm10, xmm8, xmm11
	vpsubsw ymm10, ymm8, ymm10
	vpsubsw xmm25, xmm23, xmm25
	vpsubsw ymm31, ymm22, ymm16
	vpsubsw xmm22, xmm27, xmm23
	vpsubsw ymm24, ymm16, ymm25
%endif
