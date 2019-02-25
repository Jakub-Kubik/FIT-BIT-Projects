file=proj5

all:
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
	zip xkubik32-fit.zip proj5.tex Makefile \
	Vyvoj_v_cislach.eps IoT.eps attack.eps \
	smart_city.eps IOT_adv.eps
