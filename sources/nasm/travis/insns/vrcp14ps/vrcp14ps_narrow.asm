	vrcp14ps xmm0, xmm6
	vrcp14ps xmm5, xmm1
	vrcp14ps ymm4, yword [0x90d]
	vrcp14ps ymm3, ymm1
	vrcp14ps zmm7, zmm6
	vrcp14ps zmm3, zmm5

%ifdef ERROR
	vrcp14ps xmm11, xmm12
	vrcp14ps ymm9, ymm10
	vrcp14ps zmm12, zmm10
	vrcp14ps xmm29, xmm20
	vrcp14ps ymm26, ymm27
	vrcp14ps zmm22, zmm28
%endif
