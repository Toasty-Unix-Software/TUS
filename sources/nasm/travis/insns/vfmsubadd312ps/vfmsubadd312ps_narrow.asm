	vfmsubadd312ps xmm3, xmm1, xmm6
	vfmsubadd312ps xmm3, xmm4, oword [0xbbc]
	vfmsubadd312ps ymm5, ymm5, ymm6
	vfmsubadd312ps ymm3, ymm0, yword [0x64b]

%ifdef ERROR
	vfmsubadd312ps xmm8, xmm8, xmm8
	vfmsubadd312ps ymm12, ymm10, ymm10
%endif
