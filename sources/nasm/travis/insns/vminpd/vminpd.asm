default rel
	vminpd xmm2, xmm3, oword [0x270]
	vminpd xmm2, oword [0x270]
	vminpd xmm4, xmm1, oword [0x153]
	vminpd ymm0, ymm6, ymm0
	vminpd ymm0, ymm0
	vminpd ymm0, ymm1, ymm6
	vminpd xmm6, xmm1, xmm1
	vminpd xmm6, xmm1
	vminpd xmm2, xmm4, xmm6
	vminpd ymm2, ymm5, yword [0xc9f]
	vminpd ymm2, yword [0xc9f]
	vminpd ymm6, ymm5, ymm1
	vminpd xmm15, xmm12, xmm15
	vminpd ymm12, ymm8, ymm14
	vminpd xmm12, xmm10, xmm9
	vminpd ymm10, ymm11, ymm13
	vminpd xmm21, xmm28, xmm19
	vminpd ymm24, ymm26, ymm27
	vminpd xmm16, xmm21, xmm29
	vminpd ymm23, ymm28, ymm25
	vminpd xmm0{k6}, xmm5, xmm4
	vminpd ymm1{k6}, ymm4, yword [0x735]
	vminpd zmm3{k6}, zmm4, zword [0xd96]
	vminpd xmm6{k2}{z}, xmm3, oword [0x713]
	vminpd ymm0{k1}{z}, ymm7, ymm7
	vminpd zmm1{k1}{z}, zmm1, zmm7
	vminpd zmm3, zmm5, zmm7, {sae}
	vminpd xmm4, xmm7, oword [eax+1]
	vminpd xmm2, xmm3, oword [eax+64]
	vminpd ymm5, ymm6, yword [eax+1]
	vminpd ymm3, ymm2, yword [eax+64]
	vminpd xmm0, xmm2, oword [eax+1]
	vminpd xmm5, xmm4, oword [eax+64]
	vminpd ymm1, ymm2, yword [eax+1]
	vminpd ymm4, ymm3, yword [eax+64]
	vminpd zmm3, zmm5, zword [eax+1]
	vminpd zmm6, zmm5, zword [eax+64]
	vminpd xmm2, xmm5, [0xf6d]
	vminpd ymm1, ymm2, [0xc73]
	vminpd xmm6, xmm3, [0xa58]
	vminpd ymm1, ymm7, [0x225]
	vminpd zmm3, zmm3, [0x9bb]
