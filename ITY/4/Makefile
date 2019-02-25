file=proj4

pdf:
	latex ${file}.tex
	bibtex ${file}
	latex ${file}.tex
	latex ${file}.tex
	dvips ${file}.dvi
	ps2pdf -sPAPERSIZE=a4 ${file}.ps
clean:
	rm ${file}.dvi
	rm ${file}.aux
	rm ${file}.ps
	rm ${file}.pdf

zip:
	zip xkubik32-fit.zip proj4.tex proj4.bib \
	Makefile csplainnat.bst