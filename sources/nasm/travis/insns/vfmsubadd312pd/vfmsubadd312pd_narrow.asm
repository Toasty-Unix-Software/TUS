	vfmsubadd312pd xmm0, xmm4, xmm2
	vfmsubadd312pd xmm0, xmm7, xmm5
	vfmsubadd312pd ymm4, ymm6, ymm6
	vfmsubadd312pd ymm7, ymm0, ymm6

%ifdef ERROR
	vfmsubadd312pd xmm11, xmm12, xmm11
	vfmsubadd312pd ymm9, ymm12, ymm11
%endif
