default rel
	vdpphps xmm4, xmm6, oword [0x9f4]
	vdpphps xmm6, xmm4, xmm3
	vdpphps ymm0, ymm6, yword [0x70b]
	vdpphps ymm5, ymm1, ymm7
	vdpphps zmm2, zmm2, zmm6
	vdpphps zmm1, zmm2, zmm7
	vdpphps xmm10, xmm10, xmm14
	vdpphps ymm13, ymm10, ymm11
	vdpphps zmm12, zmm9, zmm8
	vdpphps xmm28, xmm25, xmm28
	vdpphps ymm18, ymm21, ymm18
	vdpphps zmm21, zmm29, zmm22
	vdpphps xmm5{k7}, xmm1, oword [0x732]
	vdpphps ymm3{k7}, ymm2, ymm5
	vdpphps zmm1{k5}, zmm7, zmm6
	vdpphps xmm3{k7}{z}, xmm5, oword [0xff1]
	vdpphps ymm0{k2}{z}, ymm1, ymm5
	vdpphps zmm0{k4}{z}, zmm3, zmm7
	vdpphps xmm0, xmm4, oword [eax+1]
	vdpphps xmm7, xmm7, oword [eax+64]
	vdpphps ymm0, ymm6, yword [eax+1]
	vdpphps ymm5, ymm5, yword [eax+64]
	vdpphps zmm1, zmm2, zword [eax+1]
	vdpphps zmm0, zmm7, zword [eax+64]
	vdpphps xmm6, xmm6, [0x36e]
	vdpphps ymm5, ymm5, [0x3f7]
	vdpphps zmm3, zmm1, [0x4f6]
