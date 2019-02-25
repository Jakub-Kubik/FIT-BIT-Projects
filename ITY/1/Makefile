NAME=proj1

all:
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm $(NAME).pdf $(NAME).ps $(NAME).dvi \
	$(NAME).log $(NAME).aux $(NAME).fls $(NAME).fdb_latexmk

zip:
	zip xkubik32.zip proj1.tex Makefile
