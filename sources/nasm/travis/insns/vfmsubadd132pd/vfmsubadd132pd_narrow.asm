	vfmsubadd132pd xmm3, xmm3, oword [0xa66]
	vfmsubadd132pd xmm7, xmm4, xmm2
	vfmsubadd132pd ymm4, ymm6, ymm4
	vfmsubadd132pd ymm7, ymm1, ymm2
	vfmsubadd132pd xmm7, xmm3, oword [0xd04]
	vfmsubadd132pd xmm7, xmm4, xmm1
	vfmsubadd132pd ymm6, ymm2, ymm3
	vfmsubadd132pd ymm1, ymm2, ymm0

%ifdef ERROR
	vfmsubadd132pd xmm9, xmm13, xmm12
	vfmsubadd132pd ymm15, ymm13, ymm12
	vfmsubadd132pd xmm14, xmm13, xmm13
	vfmsubadd132pd ymm12, ymm13, ymm14
	vfmsubadd132pd xmm20, xmm23, xmm19
	vfmsubadd132pd ymm17, ymm28, ymm23
	vfmsubadd132pd xmm22, xmm26, xmm23
	vfmsubadd132pd ymm25, ymm30, ymm23
%endif
