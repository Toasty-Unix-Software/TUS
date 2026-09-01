	pmaxub mm2, mm0
	pmaxub mm6, mm1
	pmaxub xmm4, xmm1
	pmaxub xmm4, xmm5

%ifdef ERROR
	pmaxub xmm11, xmm12
%endif
