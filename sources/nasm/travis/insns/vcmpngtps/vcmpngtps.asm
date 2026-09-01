default rel
	vcmpngtps xmm3, xmm0, xmm7
	vcmpngtps xmm3, xmm7
	vcmpngtps xmm2, xmm7, oword [0x308]
	vcmpngtps ymm6, ymm0, ymm3
	vcmpngtps ymm6, ymm3
	vcmpngtps ymm7, ymm2, ymm4
	vcmpngtps k5, xmm3, xmm2
	vcmpngtps k3, xmm5, oword [0xa6d]
	vcmpngtps k7, ymm4, yword [0x3c9]
	vcmpngtps k6, ymm6, ymm0
	vcmpngtps xmm9, xmm9, xmm10
	vcmpngtps ymm12, ymm8, ymm13
	vcmpngtps k7, xmm11, xmm10
	vcmpngtps k7, ymm10, ymm15
	vcmpngtps k1{k2}, xmm1, xmm4
	vcmpngtps k3{k6}, ymm7, yword [0xcc1]
	vcmpngtps k4{k6}, zmm6, zmm0
	vcmpngtps k2, zmm4, zmm5, {sae}
	vcmpngtps xmm0, xmm2, oword [eax+1]
	vcmpngtps xmm1, xmm7, oword [eax+64]
	vcmpngtps ymm2, ymm7, yword [eax+1]
	vcmpngtps ymm2, ymm6, yword [eax+64]
	vcmpngtps k6, xmm1, oword [eax+1]
	vcmpngtps k7, xmm3, oword [eax+64]
	vcmpngtps k2, ymm7, yword [eax+1]
	vcmpngtps k5, ymm2, yword [eax+64]
	vcmpngtps k3, zmm0, zword [eax+1]
	vcmpngtps k2, zmm1, zword [eax+64]
	vcmpngtps xmm1, xmm0, [0x8b3]
	vcmpngtps ymm6, ymm7, [0xf31]
	vcmpngtps k4, xmm3, [0x7ad]
	vcmpngtps k3, ymm5, [0xabd]
	vcmpngtps k6, zmm5, [0xf2f]
