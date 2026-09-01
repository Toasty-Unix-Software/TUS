	vcmptrueps xmm6, xmm3, xmm4
	vcmptrueps xmm6, xmm4
	vcmptrueps xmm0, xmm5, xmm7
	vcmptrueps ymm0, ymm7, ymm3
	vcmptrueps ymm0, ymm3
	vcmptrueps ymm5, ymm3, ymm6
	vcmptrueps k3, xmm5, xmm7
	vcmptrueps k5, xmm0, xmm1
	vcmptrueps k1, ymm4, yword [0x3de]
	vcmptrueps k2, ymm6, yword [0x335]

%ifdef ERROR
	vcmptrueps xmm13, xmm13, xmm15
	vcmptrueps ymm11, ymm12, ymm8
	vcmptrueps k4, xmm11, xmm13
	vcmptrueps k5, ymm11, ymm13
%endif
