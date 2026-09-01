default rel
	vxorps xmm7, xmm1, xmm6
	vxorps xmm7, xmm6
	vxorps xmm6, xmm7, xmm6
	vxorps ymm7, ymm0, ymm0
	vxorps ymm7, ymm0
	vxorps ymm6, ymm0, ymm6
	vxorps xmm4, xmm7, xmm0
	vxorps xmm4, xmm0
	vxorps xmm5, xmm3, xmm5
	vxorps ymm0, ymm4, ymm5
	vxorps ymm0, ymm5
	vxorps ymm5, ymm7, ymm0
	vxorps xmm10, xmm8, xmm12
	vxorps ymm14, ymm12, ymm11
	vxorps xmm9, xmm14, xmm12
	vxorps ymm13, ymm11, ymm15
	vxorps xmm24, xmm17, xmm31
	vxorps ymm23, ymm27, ymm26
	vxorps xmm25, xmm26, xmm16
	vxorps ymm26, ymm28, ymm21
	vxorps xmm6{k2}, xmm3, xmm0
	vxorps ymm2{k3}, ymm3, ymm6
	vxorps zmm4{k6}, zmm6, zword [0xf83]
	vxorps xmm6{k6}{z}, xmm2, xmm5
	vxorps ymm1{k3}{z}, ymm6, yword [0xa99]
	vxorps zmm4{k4}{z}, zmm7, zmm3
	vxorps xmm6, xmm4, oword [eax+1]
	vxorps xmm3, xmm7, oword [eax+64]
	vxorps ymm1, ymm7, yword [eax+1]
	vxorps ymm4, ymm6, yword [eax+64]
	vxorps xmm0, xmm7, oword [eax+1]
	vxorps xmm7, xmm5, oword [eax+64]
	vxorps ymm6, ymm6, yword [eax+1]
	vxorps ymm6, ymm2, yword [eax+64]
	vxorps zmm5, zmm6, zword [eax+1]
	vxorps zmm1, zmm6, zword [eax+64]
	vxorps xmm7, xmm7, [0x30f]
	vxorps ymm7, ymm3, [0x19d]
	vxorps xmm7, xmm0, [0x249]
	vxorps ymm3, ymm2, [0x171]
	vxorps zmm0, zmm4, [0xc34]
