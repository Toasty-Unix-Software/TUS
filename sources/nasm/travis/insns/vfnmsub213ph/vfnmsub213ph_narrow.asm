	vfnmsub213ph xmm5, xmm4, oword [0xddf]
	vfnmsub213ph xmm5, oword [0xddf]
	vfnmsub213ph xmm7, xmm4, oword [0xdad]
	vfnmsub213ph ymm5, ymm3, ymm5
	vfnmsub213ph ymm5, ymm5
	vfnmsub213ph ymm7, ymm5, ymm0
	vfnmsub213ph zmm0, zmm2, zmm2
	vfnmsub213ph zmm0, zmm2
	vfnmsub213ph zmm4, zmm0, zword [0x337]

%ifdef ERROR
	vfnmsub213ph xmm9, xmm9, xmm15
	vfnmsub213ph ymm12, ymm10, ymm12
	vfnmsub213ph zmm10, zmm12, zmm9
	vfnmsub213ph xmm19, xmm25, xmm20
	vfnmsub213ph ymm23, ymm27, ymm21
	vfnmsub213ph zmm31, zmm17, zmm18
%endif
