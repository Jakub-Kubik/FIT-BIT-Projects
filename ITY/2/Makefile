NAME=proj2

all:
	vlna -l -v KkSsVvZzOoUuAaIi $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm $(NAME).pdf $(NAME).ps $(NAME).dvi $(NAME).te~ $(NAME).log\
	$(NAME).log $(NAME).aux $(NAME).fls $(NAME).fdb_latexmk $(NAME).synctex.gz

zip:
	zip xkubik32-fit.zip proj2.tex Makefile
