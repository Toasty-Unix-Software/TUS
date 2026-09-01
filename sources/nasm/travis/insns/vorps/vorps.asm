default rel
	vorps xmm2, xmm2, xmm5
	vorps xmm2, xmm5
	vorps xmm5, xmm1, xmm4
	vorps ymm2, ymm2, ymm2
	vorps ymm2, ymm2
	vorps ymm2, ymm1, ymm4
	vorps xmm6, xmm3, oword [0x8a7]
	vorps xmm6, oword [0x8a7]
	vorps xmm2, xmm4, oword [0x2c1]
	vorps ymm0, ymm5, ymm6
	vorps ymm0, ymm6
	vorps ymm1, ymm3, ymm1
	vorps xmm13, xmm14, xmm8
	vorps ymm14, ymm13, ymm12
	vorps xmm14, xmm10, xmm15
	vorps ymm13, ymm15, ymm8
	vorps xmm27, xmm22, xmm17
	vorps ymm18, ymm30, ymm22
	vorps xmm21, xmm19, xmm20
	vorps ymm29, ymm23, ymm25
	vorps xmm4{k5}, xmm6, xmm2
	vorps ymm2{k5}, ymm7, yword [0x9cc]
	vorps zmm1{k1}, zmm4, zword [0x737]
	vorps xmm6{k2}{z}, xmm4, xmm7
	vorps ymm6{k1}{z}, ymm1, yword [0xba6]
	vorps zmm0{k6}{z}, zmm1, zmm2
	vorps xmm7, xmm6, oword [eax+1]
	vorps xmm0, xmm1, oword [eax+64]
	vorps ymm3, ymm2, yword [eax+1]
	vorps ymm4, ymm4, yword [eax+64]
	vorps xmm1, xmm7, oword [eax+1]
	vorps xmm1, xmm3, oword [eax+64]
	vorps ymm1, ymm0, yword [eax+1]
	vorps ymm3, ymm4, yword [eax+64]
	vorps zmm0, zmm1, zword [eax+1]
	vorps zmm0, zmm1, zword [eax+64]
	vorps xmm7, xmm7, [0x794]
	vorps ymm5, ymm0, [0x570]
	vorps xmm5, xmm1, [0x282]
	vorps ymm3, ymm2, [0x645]
	vorps zmm3, zmm5, [0x8c6]
