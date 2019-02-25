-- Skript pre vytvorenie tabuliek
-- Skolsky projekt do predmetu IDS (VUT FIT)
-- Autori: Peter Kapicak (xkapic02)
--          Jakub Kubik (xkubik32)


DROP TABLE produkty_spolu CASCADE CONSTRAINTS;
DROP TABLE o_produkte CASCADE CONSTRAINTS;
DROP TABLE zaznamy CASCADE CONSTRAINTS;
DROP TABLE doplnky CASCADE CONSTRAINTS;
DROP TABLE kostymi CASCADE CONSTRAINTS;
DROP TABLE produkty CASCADE CONSTRAINTS;
DROP TABLE zakaznici CASCADE CONSTRAINTS;
DROP TABLE zamestnanci;

-- sekvencie na generovanie PK
DROP SEQUENCE  seq_produkt;
DROP SEQUENCE  seq_zaz;
-- index na optimalizaciu dotazu
drop index index_opt;

DROP TRIGGER TrigPKprodukty;
DROP TRIGGER TrigPKkostymy;
DROP TRIGGER TrigPKdoplnky;
DROP TRIGGER TrigCheckProduktValidity;

DROP MATERIALIZED VIEW MZamestnanec_registroval;

-- %%%%%%%%%%%%%%%% Tabulka Zamestnanci %%%%%%%%%%%%%%%%

CREATE TABLE zamestnanci (
    login VARCHAR2(8) NOT NULL,
    meno VARCHAR2(20) NOT NULL,
    priezvisko VARCHAR2(20) NOT NULL,
    datum_narodenia DATE NOT NULL,
    heslo VARCHAR2(64) NOT NULL,
    email VARCHAR2(64) NOT NULL,
    pozicia VARCHAR2(50) NOT NULL
);
-- Primarne kluce
ALTER TABLE zamestnanci ADD CONSTRAINT pk_zamestnanci PRIMARY KEY (login);

-- %%%%%%%%%%%%%%%% Tabulka Zakaznici %%%%%%%%%%%%%%%%

CREATE TABLE zakaznici (
    login VARCHAR2(8) NOT NULL,
    meno VARCHAR2(20) NOT NULL,
    priezvisko VARCHAR2(20) NOT NULL,
    heslo VARCHAR2(64) NOT NULL,
    email VARCHAR2(64) NOT NULL,
    registroval VARCHAR2(8) NOT NULL
);
-- Primarne kluce
ALTER TABLE zakaznici ADD CONSTRAINT pk_zakaznici PRIMARY KEY (login);
-- Cudzie kluce
ALTER TABLE zakaznici ADD CONSTRAINT fk_registroval
                        FOREIGN KEY (registroval)
                        REFERENCES zamestnanci(login)
                        ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka Produkty %%%%%%%%%%%%%%%%

CREATE TABLE produkty (
    produkt_id INT,     -- NOT NULL bolo povodne ale koli trigeru na inkrementaciu id som odstanil
    opotrebenie VARCHAR2(20),
    datum_vyroby DATE,
    spravca VARCHAR2(8) NOT NULL
);
-- Primarne kluce
ALTER TABLE produkty ADD CONSTRAINT pk_produkty PRIMARY KEY (produkt_id);
-- Cudzie kluce
ALTER TABLE produkty ADD CONSTRAINT fk_spravca
                        FOREIGN KEY (spravca)
                        REFERENCES zamestnanci(login)
                        ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka Kostymi %%%%%%%%%%%%%%%%

CREATE TABLE kostymi (
    produkt_id INT, -- NOT NULL bolo povodne ale koli trigeru na inkrementaciu id som odstanil
    popis VARCHAR2(128) NOT NULL,
    vyrobca VARCHAR2(32),
    material VARCHAR2(32),
    farba VARCHAR2(16),
    velkost VARCHAR2(16) NOT NULL,
    kategoria VARCHAR2(16)
);
-- Primarne kluce
ALTER TABLE kostymi ADD CONSTRAINT pk_kostymi PRIMARY KEY (produkt_id);
-- Cudzie kluce
ALTER TABLE kostymi ADD CONSTRAINT fk_kostymi
                    FOREIGN KEY (produkt_id)
                    REFERENCES produkty(produkt_id)
                    ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka Doplnky %%%%%%%%%%%%%%%%

CREATE TABLE doplnky (
    produkt_id INT, -- NOT NULL bolo povodne ale koli trigeru na inkrementaciu id som odstanil
    nazov VARCHAR2(32) NOT NULL,
    popis VARCHAR2(128)
);
-- Primarne kluce
ALTER TABLE doplnky ADD CONSTRAINT pk_doplnky PRIMARY KEY (produkt_id);
-- Cudzie kluce
ALTER TABLE doplnky ADD CONSTRAINT fk_doplnky
                    FOREIGN KEY (produkt_id)
                    REFERENCES produkty(produkt_id)
                    ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka Zaznamy %%%%%%%%%%%%%%%%

CREATE TABLE zaznamy (
    zaznam_id INT NOT NULL,
    akcia VARCHAR2(32),
    datum_pozicky DATE NOT NULL,
    datum_vratenia DATE NOT NULL,
    cena FLOAT NOT NULL,
    zakaznik VARCHAR2(8) NOT NULL,
    zamestnanec VARCHAR2(8) NOT NULL
);
ALTER TABLE zaznamy ADD CHECK (cena > 0);
-- Primarne kluce
ALTER TABLE zaznamy ADD CONSTRAINT pk_zaznamy PRIMARY KEY (zaznam_id);
-- Cudzie kluce
ALTER TABLE zaznamy ADD CONSTRAINT fk_zamestnanec
                    FOREIGN KEY (zamestnanec)
                    REFERENCES zamestnanci(login)
                    ON DELETE CASCADE;

ALTER TABLE zaznamy ADD CONSTRAINT fk_zakaznik
                    FOREIGN KEY (zakaznik)
                    REFERENCES zakaznici(login)
                    ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka O produkte %%%%%%%%%%%%%%%%

CREATE TABLE o_produkte (
    produkt_id INT NOT NULL,
    zaznam_id INT NOT NULL
);
-- Primarne kluce
ALTER TABLE o_produkte ADD CONSTRAINT pk_o_produkte PRIMARY KEY (produkt_id, zaznam_id);
-- Cudzie kluce
ALTER TABLE o_produkte ADD CONSTRAINT fk_o_produkt
                    FOREIGN KEY (produkt_id)
                    REFERENCES produkty(produkt_id)
                    ON DELETE CASCADE;

ALTER TABLE o_produkte ADD CONSTRAINT fk_o_zaznam
                    FOREIGN KEY (zaznam_id)
                    REFERENCES zaznamy(zaznam_id)
                    ON DELETE CASCADE;

-- %%%%%%%%%%%%%%%% Tabulka produkty spolu %%%%%%%%%%%%%%%%

CREATE TABLE produkty_spolu (
    kostym_id INT NOT NULL,
    doplnok_id INT NOT NULL
);
-- Primarne kluce
ALTER TABLE produkty_spolu ADD CONSTRAINT pk_produkty_spolu PRIMARY KEY (kostym_id, doplnok_id);
-- Cudzie kluce
ALTER TABLE produkty_spolu ADD CONSTRAINT fk_doplnok
                    FOREIGN KEY (doplnok_id)
                    REFERENCES doplnky(produkt_id)
                    ON DELETE CASCADE;

ALTER TABLE produkty_spolu ADD CONSTRAINT fk_kostym
                    FOREIGN KEY (kostym_id)
                    REFERENCES kostymi(produkt_id)
                    ON DELETE CASCADE;

-- ===========================================================================================================
-- ===========================================================================================================

CREATE SEQUENCE seq_produkt
                MINVALUE 1
                START WITH 1
                INCREMENT BY 1;

CREATE SEQUENCE seq_zaz
                MINVALUE 1
                START WITH 1
                INCREMENT BY 1;

-- Pridavanie do tabulky zamestnanci

INSERT INTO zamestnanci (login, meno, priezvisko, datum_narodenia, heslo, email, pozicia)
VALUES ('xkapic02', 'Peter', 'Kapicak', '28.02.1997', 'ids', 'xkapick02@stud.fit.vutbr.cz', 'majitel');

INSERT INTO zamestnanci (login, meno, priezvisko, datum_narodenia, heslo, email, pozicia)
VALUES ('xkubik32', 'Jakub', 'Kubik', '13.07.1996', 'ids', 'xkubik32@stud.fit.vutbr.cz', 'majitel');

INSERT INTO zamestnanci (login, meno, priezvisko, datum_narodenia, heslo, email, pozicia)
VALUES ('xhazuc00', 'Ivan', 'Hazucha', '19.08.1997', 'ids', 'xhazuc00@stud.fit.vutbr.cz', 'prevadzkar');

INSERT INTO zamestnanci (login, meno, priezvisko, datum_narodenia, heslo, email, pozicia)
VALUES ('xkapis00', 'Marian', 'Kapisinsky', '21.06.1998', 'ids', 'xkapis00@stud.fit.vutbr.cz', 'skladnik');

-- Pridavanie do tabulky zakaznici

INSERT INTO zakaznici (login, meno, priezvisko, heslo, email, registroval)
VALUES ('ymessi01', 'Lionel', 'Messi', 'booo', 'my.precious@gmail.com', 'xkapic02');

INSERT INTO zakaznici (login, meno, priezvisko, heslo, email, registroval)
VALUES ('yjasni23', 'Matus', 'Jasnicky', 'koder', 'jasnicky.m@gmail.com', 'xkubik32');

INSERT INTO zakaznici (login, meno, priezvisko, heslo, email, registroval)
VALUES ('ydeakt23', 'Tomas', 'Deak', 'fighter', 'deak@gmail.com', 'xkubik32');

INSERT INTO zakaznici (login, meno, priezvisko, heslo, email, registroval)
VALUES ('ygerra23', 'Steven', 'Gerrard', 'legend', 'stevie.g@gmail.com', 'xkapic02');

INSERT INTO zakaznici (login, meno, priezvisko, heslo, email, registroval)
VALUES ('yvandi23', 'Virgil', 'van Dijk', 'leader', 'virgil.v.d@gmail.com', 'xhazuc00');

-- Pridavanie do tabulky zaznamy/produkty

INSERT INTO zaznamy (zaznam_id, akcia, datum_pozicky, datum_vratenia, cena, zakaznik, zamestnanec)
VALUES (seq_zaz.nextval, 'Karneval Borovce', '25.02.2018', '11.03.2018', 15.50, 'ymessi01', 'xkapic02');

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'male', '14.07.2016', 'xkubik32');

INSERT INTO kostymi (produkt_id, popis, vyrobca, material, farba, velkost, kategoria)
VALUES (seq_produkt.currval, 'Spiderman kostym pre dospelych', 'Mask for you s.r.o.', 'polyester', 'cervena', 'L', 'superhrdinovia');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);



INSERT INTO zaznamy (zaznam_id, akcia, datum_pozicky, datum_vratenia, cena, zakaznik, zamestnanec)
VALUES (seq_zaz.nextval, 'Karneval Piestany', '21.02.2018', '07.03.2018', 19.50, 'ydeakt23', 'xkapic02');

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'male', '02.11.2017', 'xkapic02');

INSERT INTO kostymi (produkt_id, popis, vyrobca, material, farba, velkost, kategoria)
VALUES (seq_produkt.currval, 'Hulk Kostym pre deti', 'BeMask s.r.o.', 'polyester', 'zelena', 'XS', 'superhrdinovia');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);



INSERT INTO zaznamy (zaznam_id, akcia, datum_pozicky, datum_vratenia, cena, zakaznik, zamestnanec)
VALUES (seq_zaz.nextval, 'Ples Priser', '11.02.2018', '1.03.2018', 25.00, 'ygerra23', 'xkapis00');

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'ziadne', '25.09.2015', 'xkapis00');

INSERT INTO kostymi (produkt_id, popis, vyrobca, material, farba, velkost, kategoria)
VALUES (seq_produkt.currval, 'Frankenstein', 'BeMask s.r.o.', 'polyester', 'cierna', 'XL', 'Filmove prisery');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);



INSERT INTO zaznamy (zaznam_id, akcia, datum_pozicky, datum_vratenia, cena, zakaznik, zamestnanec)
VALUES (seq_zaz.nextval, 'Detsky karneval O2', '04.02.2018', '10.03.2018', 11.50, 'yvandi23', 'xhazuc00');

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'ziadne', '06.05.2016', 'xkubik32');

INSERT INTO kostymi (produkt_id, popis, vyrobca, material, farba, velkost, kategoria)
VALUES (seq_produkt.currval, 'Musketier', 'Mask for you s.r.o.', 'polyester', 'cierna', 'XXS', 'bojovnici');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'male', '02.11.2015', 'xkapic02');

INSERT INTO doplnky (produkt_id, nazov, popis)
VALUES (seq_produkt.currval, 'Musketierov mec', 'Dreveny mec k maske Musketiera');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);

INSERT INTO produkty_spolu (kostym_id, doplnok_id)
VALUES ((SELECT produkt_id
              FROM kostymi
              WHERE popis='Musketier'), seq_produkt.currval);


INSERT INTO zaznamy (zaznam_id, akcia, datum_pozicky, datum_vratenia, cena, zakaznik, zamestnanec)
VALUES (seq_zaz.nextval, 'Karneval Piestany', '02.02.2018', '07.03.2018', 35, 'ydeakt23', 'xkapic02');

INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES (seq_produkt.nextval, 'stredne', '07.08.2017', 'xkapis00');

INSERT INTO doplnky (produkt_id, nazov, popis)
VALUES (seq_produkt.currval, 'Celenka pre princeznu', 'Celene pre malu princeznu');

INSERT INTO o_produkte (produkt_id, zaznam_id)
VALUES (seq_produkt.currval,seq_zaz.currval);


COMMIT;

GRANT ALL ON zamestnanci TO xkubik32;
GRANT ALL ON zakaznici TO xkubik32;
GRANT ALL ON zaznamy TO xkubik32;
GRANT ALL ON kostymi TO xkubik32;
GRANT ALL ON doplnky TO xkubik32;
GRANT ALL ON produkty TO xkubik32;
GRANT ALL ON o_produkte TO xkubik32;
GRANT ALL ON produkty_spolu TO xkubik32;

-- --------------------------------------------------
-- DOTAZY 3. CAST PROJEKTU
-- --------------------------------------------------

-- 1. DOTAZ:  SPOJENIE 2 TABULIEK
--            vsetky  opotrebenia kostymov (nie doplnky) a popis kostymov
--            co spravuje zamestnanec xkubik32 (SPOJENIE 2 TABULIEK)

    SELECT produkty.opotrebenie, kostymi.popis
        FROM Produkty
            INNER JOIN Kostymi ON kostymi.produkt_id = produkty.produkt_id
        WHERE produkty.spravca = 'xkubik32';


  -- 2. DOTAZ:  SPOJRNIE 2 TABULIEK
  --            nazov kostymu ktory vyrobila firma Mask for you s.r.o a bol pozicany uz nejakou osobou
  --            tiez meno tej osoby kt si kostym pozicala

  SELECT kostymi.popis, zaznamy.zakaznik
    FROM kostymi
        INNER JOIN zaznamy ON zaznamy.zaznam_id = kostymi.produkt_id
    WHERE kostymi.vyrobca = 'Mask for you s.r.o.';


  -- 3. DOTAZ:  SPOJENIE 3 TABULIEK
  --            kostymy a doplnky ktore sa k patira k sebe

  SELECT k.popis as kostym, d.nazov as doplnok
  FROM  produkty p
    INNER JOIN produkty_spolu s ON s.kostym_id = p.produkt_id
    INNER JOIN kostymi k ON k.produkt_id = s.kostym_id,
    produkty p
    INNER JOIN produkty_spolu s ON s.doplnok_id = p.produkt_id
    INNER JOIN doplnky d ON d.produkt_id = s.doplnok_id;


  -- 4. DOTAZ: KLAUZULA GROUP BY A AGREGACNA FUNKCIA
  --           na ktoru akciu, ktory zakaznik a kolko
  --           zaplatil zakaznik viac ako je priemerna cena zakaznika za akciu

  SELECT AVG(zaznamy.cena) as Priemerna_cena FROM zaznamy ;
  SELECT z.akcia, z.zakaznik, z.cena
    FROM zaznamy z
    WHERE z.cena > (SELECT AVG(c.cena) FROM zaznamy c )
    GROUP BY z.zakaznik, z.akcia, z.cena;


  -- 5. DOTAZ: KLAUZULA GRUPBY A AGREGACNA FUNKCIA
  --           celkova suma ktoru zaplatili zakaznici za poziciavanie
  --            meno priezvisko zakaznika a cena
 SELECT  zak.meno, zak.priezvisko, SUM(zaz.cena) as cena
  FROM zaznamy zaz
  INNER JOIN zakaznici zak ON zak.login = zaz.zakaznik
  GROUP BY zak.meno, zak.priezvisko;

   -- 6. DOTAZ: PREDIKAT EXISTS
  --           Vypis nazvy vsetkch kostymov ktore nemaju ziadne doplnky
  SELECT k.popis AS kostym
  FROM kostymi k
  WHERE NOT EXISTS (SELECT p.kostym_id FROM produkty_spolu p WHERE p.kostym_id = k.produkt_id);


  -- 7. DOTAZ: PREDIKAT IN
  --           vybrat zaznam id a akciu spojenu s tymto zaznamom
  --           z mnoziny zaznamov ktorych je cena vyssia ako 13
  SELECT z.zaznam_id, z.akcia
  FROM zaznamy z
  WHERE z.cena IN(SELECT c.cena FROM zaznamy c WHERE c.cena > 13);


-- --------------------------------------------------
-- 4. CAST PROJEKTU
-- --------------------------------------------------

-- -------------------------------------------------------------------------------------------------
-- Trigger c.1 pre automaticke generovanie hodnot PK tabulky produkty, kostymi, doplnky pri PK NULL
-- -------------------------------------------------------------------------------------------------

-- automaticka aktualizaci pk produktu ak vlozeny PK je NULL
CREATE OR REPLACE TRIGGER TrigPKprodukty
BEFORE INSERT ON produkty
FOR EACH ROW
    WHEN (new.produkt_id IS NULL)
    BEGIN
        :new.produkt_id := seq_produkt.nextval;
    END;
/
-- automaticka aktualizacia pk kostymu ak vlozeny PK je NULL
CREATE OR REPLACE TRIGGER TrigPKkostymi
BEFORE INSERT ON kostymi
FOR EACH ROW
    WHEN (new.produkt_id IS NULL)
    BEGIN
        :new.produkt_id := seq_produkt.currval;
    END;
/
-- automaticka aktualizacia pk doplnok ak vlozeny pk je NULL
CREATE OR REPLACE TRIGGER TrigPKdoplnky
BEFORE INSERT ON doplnky
FOR EACH ROW
    WHEN (new.produkt_id IS NULL)
    BEGIN
        :new.produkt_id := seq_produkt.currval;
    END;
/

-- otestovanie funkcosti 1. a 2. triggeru
INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES ( NULL, 'velke', '03.3.2018', 'xkubik32');
INSERT INTO kostymi (produkt_id, popis, vyrobca, material, farba, velkost, kategoria)
VALUES (NULL, 'Vojak', 'Mask for you s.r.o.', 'polyester', 'cierna', 'XXL', 'bojovnici');

SELECT * FROM produkty;
SELECT * FROM kostymi;

-- otestovanie funkcnosti 1. a 3. triggeru
INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES ( NULL, 'male', '02.11.2017', 'xkapic02');
INSERT INTO doplnky (produkt_id, nazov, popis)
VALUES (NULL, 'Samopal', 'Zbran pre vojaka');

SELECT * FROM produkty;
SELECT * FROM doplnky;

-- ------------------------------------------------------------------------------
-- Trigger c.2 pre skontrolovanie, ci je maximalne jeden produkt, ku ktoremu
--                                  nie je prideleny ani kostym ani doplnok
--                                  inak sa vyvola chyba aplikacie
-- ------------------------------------------------------------------------------
CREATE OR REPLACE TRIGGER TrigCheckProduktValidity
AFTER INSERT ON produkty
DECLARE pocet INT;
    BEGIN
        SELECT count(*)
        INTO pocet
        FROM produkty P
        WHERE P.produkt_id NOT IN ( SELECT produkt_id FROM kostymi ) AND
              P.produkt_id NOT IN ( SELECT produkt_id FROM doplnky );
        IF pocet > 1 THEN
            RAISE_APPLICATION_ERROR(42, 'Tabulka produkty ma viac ako 1 primarny kluc ku ktoremu nie je priradeny ani kostym ani doplnok');
        END IF;
    END;
/

-- povolene vlozit jeden produktku ktoremu nie je prideleny ani kostym ani doplnok
INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
VALUES ( NULL, 'male', '02.11.2017', 'xkapic02');

-- vyvolanie chyby pri vkladani druheho prvku ku ktoremu nie  je prideleny ani doplnok ani kostym
-- INSERT INTO produkty (produkt_id, opotrebenie, datum_vyroby, spravca)
-- VALUES ( NULL, 'male', '02.11.2017', 'xkapic02');


-- ----------------------------------------------------------------------------------
-- EXPLAIN PLAN a optimaliziacia pomocou pridania indexu
-- -----------------------------------------------------------------------------------

-- plan vykonavania dotazu bez optimalizacie
EXPLAIN PLAN FOR
SELECT Z.meno, Z.priezvisko, COUNT(*) AS spravuje_pocet
FROM zamestnanci Z NATURAL JOIN produkty P
WHERE Z.login = P.spravca
GROUP BY Z.meno, Z.priezvisko;
SELECT * FROM TABLE(dbms_xplan.display);

-- tvorba indexu pre optimalizaciu
CREATE INDEX index_opt ON produkty(spravca);
-- plan vykonavania dotazu s optimalizaciou
EXPLAIN PLAN FOR
SELECT Z.meno, Z.priezvisko, COUNT(*) AS spravuje_pocet
FROM zamestnanci Z NATURAL JOIN produkty P
WHERE Z.login = P.spravca
GROUP BY Z.meno, Z.priezvisko;
SELECT * FROM TABLE(dbms_xplan.display);

-- ------------------------------------------------------------------------------
-- Procedura c.1 na zistenie kolko percent produktov spravuje zamestnanec
-- ------------------------------------------------------------------------------
SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE ZamSpravuje
(login varchar2) IS
SpravujePocet NUMBER;
CelkovyPocetKost NUMBER;
CelkovyPocetDop NUMBER;
CelkovyPocet NUMBER;
Percenta NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO SpravujePocet
    FROM Produkty P
    WHERE P.spravca = login;

    SELECT COUNT(*)
    INTO CelkovyPocetKost
    FROM kostymi;

    SELECT COUNT(*) 
    INTO CelkovyPocetDop
    FROM doplnky;

    CelkovyPocet := CelkovyPocetKost + CelkovyPocetDop;
    Percenta := (SpravujePocet / (CelkovyPocetKost + CelkovyPocetDop)) * 100;
    DBMS_OUTPUT.put_line(to_char( Percenta ) || '% spravuje ' || login   || ' zo vsetkeho tovaru');
    EXCEPTION
    WHEN zero_divide THEN
    DBMS_OUTPUT.put_line('Delenie nulou - nie su ziadne doplnky ani kostymi');
END ZamSpravuje;
/

-- otestovanie funkcnosti prvej procedury
EXECUTE ZamSpravuje('xkubik32');
EXECUTE ZamSpravuje('xkapic02');

-- ----------------------------------------------------------------------------------------------------
-- Procedura c.2 zmena hesla zamestnanca
--               explicitny kurzor a premennu s dat. typom odkazujucim na riadok tabulky zamestnanec
-- ----------------------------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE ZamZmenaHesla
(login varchar2, newPasswd varchar2) IS
    found_zam BOOLEAN := FALSE;
    cur_zam_login zamestnanci.login%type;
    CURSOR cur_zamestnanci is 
      SELECT login FROM zamestnanci; 
BEGIN 
   OPEN cur_zamestnanci; 
   LOOP 
   FETCH cur_zamestnanci into cur_zam_login;
   EXIT WHEN cur_zamestnanci%notfound; 
      IF cur_zam_login = login THEN 
            found_zam := TRUE;
            update zamestnanci set heslo=newPasswd where cur_zam_login = login;
      END IF;
   END LOOP; 
    CLOSE cur_zamestnanci; 
    IF found_zam THEN
        DBMS_OUTPUT.put_line('Zanestnanec ' || login || ' bol najdeny a jeho heslo zmenene' );
    ELSE
        DBMS_OUTPUT.put_line('Zanestnanec ' || login || ' nepracuje v tejto pozicovni kostymov' );
    END IF;   
END; 
/

-- otestovanie funkcnosti druhej procedury
SELECT * FROM zamestnanci;
EXECUTE ZamZmenaHesla('xkubik32', 'sds');
EXECUTE ZamZmenaHesla('xkubik30', 'sadsf');
SELECT * FROM zamestnanci;


-- ------------------------------------------------------------------------------
-- Materializovany pohlad pre zoznam zamestnancov a ludi ktorich registrovali
-- ------------------------------------------------------------------------------
CREATE MATERIALIZED VIEW MZamestnanec_registroval (zam_login, zam_meno, zam_priezvisko, zam_pozicia,
                                                    reg_login, reg_meno, reg_priezvisko)
CACHE
REFRESH ON COMMIT 
AS
   SELECT zam.login, zam.meno, zam.priezvisko, zam.pozicia,
          zak.login, zak.meno, zak.priezvisko
    FROM zamestnanci zam, zakaznici zak
    WHERE zam.login = zak.registroval;

-- povolenie SELECT druhemu clenovi timu k materializovanemu pohladu MZamestnanec_registroval
GRANT SELECT ON MZamestnanec_registroval TO xkapic02;

-- toto pojde vo verzii pre odovzdanie prec
SELECT * FROM MZamestnanec_registroval;
