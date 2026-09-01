	vfmaddsub132pd xmm0, xmm6, xmm1
	vfmaddsub132pd xmm4, xmm4, xmm2
	vfmaddsub132pd ymm7, ymm0, yword [0x2e9]
	vfmaddsub132pd ymm6, ymm6, yword [0xb2f]
	vfmaddsub132pd xmm6, xmm2, oword [0xae6]
	vfmaddsub132pd xmm7, xmm4, xmm7
	vfmaddsub132pd ymm4, ymm7, ymm4
	vfmaddsub132pd ymm4, ymm7, ymm2

%ifdef ERROR
	vfmaddsub132pd xmm14, xmm15, xmm12
	vfmaddsub132pd ymm11, ymm12, ymm9
	vfmaddsub132pd xmm13, xmm15, xmm8
	vfmaddsub132pd ymm11, ymm9, ymm8
	vfmaddsub132pd xmm16, xmm27, xmm25
	vfmaddsub132pd ymm26, ymm19, ymm28
	vfmaddsub132pd xmm17, xmm27, xmm17
	vfmaddsub132pd ymm23, ymm25, ymm17
%endif
