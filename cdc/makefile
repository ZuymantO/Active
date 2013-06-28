PDF=pdflatex cdc
GLOS=makeglossaries cdc.glo
IDX=makeindex cdc

all :
	$(PDF)
	$(PDF)
	$(GLOS)
	$(PDF)
	$(IDX)
	$(PDF)
	$(PDF)

cleanall : clean
	rm -rf cdc.pdf

clean : cleanCDC cleanGlossaire
	rm -rf *~

cleanCDC :
	rm -rf cdc.aux cdc.glg cdc.glo cdc.log cdc.out cdc.out cdc.toc cdc.xdy cdc.gls cdc.acn cdc.bbl cdc.blg cdc.idx cdc.ind cdc.ilg

cleanGlossaire :
	rm -rf glossaire.aux glossaire.log