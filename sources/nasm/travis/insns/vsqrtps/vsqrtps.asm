default rel
	vsqrtps xmm4, oword [0x615]
	vsqrtps xmm4, xmm2
	vsqrtps ymm7, ymm4
	vsqrtps ymm0, ymm6
	vsqrtps xmm6, xmm5
	vsqrtps xmm2, xmm4
	vsqrtps ymm2, ymm0
	vsqrtps ymm0, ymm4
	vsqrtps xmm10, xmm8
	vsqrtps ymm8, ymm12
	vsqrtps xmm9, xmm10
	vsqrtps ymm15, ymm11
	vsqrtps xmm23, xmm20
	vsqrtps ymm30, ymm31
	vsqrtps xmm27, xmm18
	vsqrtps ymm23, ymm27
	vsqrtps xmm4{k4}, xmm3
	vsqrtps ymm7{k6}, ymm1
	vsqrtps zmm3{k3}, zmm1
	vsqrtps xmm5{k5}{z}, xmm5
	vsqrtps ymm5{k1}{z}, ymm4
	vsqrtps zmm0{k7}{z}, zmm7
	vsqrtps zmm2, zmm5, {rn-sae}
	vsqrtps xmm6, oword [eax+1]
	vsqrtps xmm3, oword [eax+64]
	vsqrtps ymm4, yword [eax+1]
	vsqrtps ymm3, yword [eax+64]
	vsqrtps xmm4, oword [eax+1]
	vsqrtps xmm6, oword [eax+64]
	vsqrtps ymm7, yword [eax+1]
	vsqrtps ymm4, yword [eax+64]
	vsqrtps zmm1, zword [eax+1]
	vsqrtps zmm7, zword [eax+64]
	vsqrtps xmm3, [0x4d8]
	vsqrtps ymm3, [0xbc7]
	vsqrtps xmm6, [0x546]
	vsqrtps ymm1, [0x1ad]
	vsqrtps zmm1, [0x4ba]
