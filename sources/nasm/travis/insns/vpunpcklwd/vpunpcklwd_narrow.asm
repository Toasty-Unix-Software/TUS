	vpunpcklwd xmm4, xmm5, oword [0x6db]
	vpunpcklwd xmm4, oword [0x6db]
	vpunpcklwd xmm5, xmm1, oword [0xeff]
	vpunpcklwd ymm1, ymm5, yword [0x611]
	vpunpcklwd ymm1, yword [0x611]
	vpunpcklwd ymm2, ymm4, ymm1
	vpunpcklwd xmm2, xmm0, xmm1
	vpunpcklwd xmm2, xmm1
	vpunpcklwd xmm2, xmm4, oword [0x52c]
	vpunpcklwd ymm6, ymm4, ymm0
	vpunpcklwd ymm6, ymm0
	vpunpcklwd ymm3, ymm5, yword [0xffb]

%ifdef ERROR
	vpunpcklwd xmm11, xmm11, xmm8
	vpunpcklwd ymm10, ymm8, ymm15
	vpunpcklwd xmm10, xmm11, xmm9
	vpunpcklwd ymm13, ymm15, ymm11
	vpunpcklwd xmm20, xmm27, xmm28
	vpunpcklwd ymm23, ymm27, ymm31
	vpunpcklwd xmm31, xmm25, xmm27
	vpunpcklwd ymm28, ymm16, ymm26
%endif
