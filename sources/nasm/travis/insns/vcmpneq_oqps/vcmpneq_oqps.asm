default rel
	vcmpneq_oqps xmm6, xmm3, xmm2
	vcmpneq_oqps xmm6, xmm2
	vcmpneq_oqps xmm2, xmm0, xmm6
	vcmpneq_oqps ymm0, ymm7, ymm6
	vcmpneq_oqps ymm0, ymm6
	vcmpneq_oqps ymm1, ymm4, ymm6
	vcmpneq_oqps k1, xmm1, xmm7
	vcmpneq_oqps k4, xmm1, xmm2
	vcmpneq_oqps k2, ymm3, ymm2
	vcmpneq_oqps k7, ymm0, ymm2
	vcmpneq_oqps xmm10, xmm11, xmm14
	vcmpneq_oqps ymm14, ymm12, ymm9
	vcmpneq_oqps k4, xmm13, xmm11
	vcmpneq_oqps k2, ymm12, ymm14
	vcmpneq_oqps k7{k6}, xmm3, oword [0x569]
	vcmpneq_oqps k5{k6}, ymm6, ymm1
	vcmpneq_oqps k3{k1}, zmm5, zword [0x60c]
	vcmpneq_oqps k2, zmm0, zmm3, {sae}
	vcmpneq_oqps xmm5, xmm2, oword [eax+1]
	vcmpneq_oqps xmm2, xmm2, oword [eax+64]
	vcmpneq_oqps ymm0, ymm3, yword [eax+1]
	vcmpneq_oqps ymm0, ymm0, yword [eax+64]
	vcmpneq_oqps k7, xmm2, oword [eax+1]
	vcmpneq_oqps k1, xmm3, oword [eax+64]
	vcmpneq_oqps k2, ymm2, yword [eax+1]
	vcmpneq_oqps k1, ymm7, yword [eax+64]
	vcmpneq_oqps k5, zmm4, zword [eax+1]
	vcmpneq_oqps k1, zmm4, zword [eax+64]
	vcmpneq_oqps xmm5, xmm6, [0x96d]
	vcmpneq_oqps ymm3, ymm0, [0xaa8]
	vcmpneq_oqps k6, xmm3, [0x420]
	vcmpneq_oqps k5, ymm3, [0x8aa]
	vcmpneq_oqps k4, zmm2, [0xcef]
