# Feedreader školský projekt na VUT FIT do predmetu ISA 

feedreader vypisuje informácie uvedené v stiahnutých zdrojoch (feed) vo formáte Atom, RSS1 a RSS2.
Program po spustení stiahne zadané zdroje a na štandardný výstup vypíše požadované informácie uživateľom.
Informácie si uživateľ určuje pomocou volitelných argumentov pre program. 

## Obmedzenia programu a prípadnné rozšírenia

Zo zadania sú splnené všetky podmienky.

## Preklad

Program sa preloží pomocou make.

## Testy

Všetky test sa spustia pomocou make test. Popis testovania je podrobne rozobraný v dokumentácii.

## Spustenie aplikácie

feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]

Poradie parametrov je ľubovolné. Popis parametrov:

    - Povinne je uvedené buď URL požadovaného zdroja (http, https) alebo parameter -f s dodatočným parametrom určujúcim umiestnenie súboru feedfile 
      (oba paramtre naraz = chyba).
    - Volitelný parameter -c definuje súbor <certfile> s certifikátmi, ktoré použije na overenie platnosti certifikátu SSL/TLS prodloženého servrom.
    - Volitelný parameter -C určuje adresár <ceraddr>, v ktorom sa majú vyhladávať certifikáty, ktoré sa použiju na overenie platnosti certifikátu SSL/TLS
      predloženého servrom.
    - Volitelný parameter -T je pre zobrazenie dodatočnej informácii o čase zmeny záznamu alebo vytvorení záznamu.
    - Volitelný parameter -a je pre zobrazenie dodatočnej informácii o mene autora záznamu, či jeho email.
    - Volitelný parameter -u je pre zobrazenie dodatočnej informácii o URL asociovanom k danému záznamu.

## Vzorové spustenie aplikácie 

./feedreader https://tools.ietf.org/dailydose/dailydose_atom.xml
\*\*\* The Daily Dose of IETF \*\*\*
The Daily Dose of IETF - Issue 3192 - 2018-09-18
The Daily Dose of IETF - Issue 3191 - 2018-09-17
...

## Odovzdaný adresár
    .
    ├── LICENSE
    ├── Makefile
    ├── manual.pdf
    ├── README
    ├── src
    │   ├── ArgumentsParser.cpp     # trieda pre parsovanie a uloženie argumentov
    │   ├── Client.cpp              # trieda pre pripojenie sa k servru a získanie obsahu (feedu)
    │   ├── FeedfileParser.cpp      # trieda pre prácu s feedfile a uloženie obsahu feedfile
    │   ├── FeedParser.cpp          # trieda pre konkrétne pársovanie Atom, RSS1 a RSS2 formátov
    │   ├── include                     
    │   │   ├── ArgumentsParser.h   # hlavičkový súbor pre triedu ArgumentsParser
    │   │   ├── Client.h            # hlavičkový súbor pre triedu Client
    │   │   ├── FeedfileParser.h    # hlavičkový súbor pre triedu FeedFileParser
    │   │   ├── FeedParser.h        # hlavičkový súbor pre triedu FeedParser
    │   │   └── main.h              # hlavičkový súbor pre moje chybové kódy
    │   └── main.cpp                # vstupná funkcia programu ktorá vytvára všetky potrebné objekty a obsahuje hlavnú logiku programu
    └── tests       
        ├── server.py   
        └── test.py         

## Autor
Ján Jakub Kubík.



