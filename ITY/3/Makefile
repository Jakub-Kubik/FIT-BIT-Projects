NAME=proj3
all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm $(NAME).pdf $(NAME).ps $(NAME).dvi \
	$(NAME).log $(NAME).aux

zip:
	zip xkubik32-fit.zip proj3.tex Makefile \
	 					etiopan.eps oniisan.eps oniisan2.eps
