	vpackssdw xmm5, xmm2, oword [0x39b]
	vpackssdw xmm5, oword [0x39b]
	vpackssdw xmm6, xmm5, xmm7
	vpackssdw ymm2, ymm3, ymm2
	vpackssdw ymm2, ymm2
	vpackssdw ymm5, ymm6, ymm5
	vpackssdw xmm6, xmm0, xmm2
	vpackssdw xmm6, xmm2
	vpackssdw xmm2, xmm1, xmm4
	vpackssdw ymm5, ymm6, ymm0
	vpackssdw ymm5, ymm0
	vpackssdw ymm3, ymm0, ymm3

%ifdef ERROR
	vpackssdw xmm14, xmm13, xmm9
	vpackssdw ymm12, ymm10, ymm8
	vpackssdw xmm12, xmm11, xmm9
	vpackssdw ymm11, ymm11, ymm13
	vpackssdw xmm26, xmm26, xmm23
	vpackssdw ymm28, ymm19, ymm19
	vpackssdw xmm20, xmm25, xmm30
	vpackssdw ymm23, ymm18, ymm25
%endif
