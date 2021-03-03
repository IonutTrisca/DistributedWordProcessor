# Distributed Word Processor
This program takes a file with different types of paragraphs and processes each one according to specific rules.
The processing is done in a distributed manner using MPI. Each type of paragraph has a thread in the master process, and a worker process which processes the paragraph.
The worker process has its own threads to process each paragraph in a parallel manner. 
The modified paragraphs are then written by the master process in the output file in the same order as the input file

## Detailed Functionality
The program accepts 4 paragraph types and processes each one with a different worker.  
  
The processing is done as follows:
* **horror** -> each consonant is doubled
* **comedy** -> each even position letter is capitalized
* **fantasy** -> first letter of each word is capitalized
* **science-fiction** -> each 7th word of a sentence is reversed
  
## Program description in Romanian
Programul functioneaza astfel:

Procesul master creeaza 4 threaduri, unul pentru fiecare gen de paragraf.
Fiecare thread apoi citeste din fisierul de intrare si de fiecare data cand
intalneste o linie care denota inceperea unui nou paragraf incrementeaza un
counter care retine cate paragrafe au fost inaintea celui curent. Acest lucru
este facut pentru a ajuta la sincronizare. 

Daca threadul gaseste un paragraf care are genul sau, citeste pana cand gaseste
urmatorul paragraf, apoi trimite acest paragraf catre procesul worker ce se
ocupa de genul respectiv. Voi detalia mai jos cum workerul proceseaza acel paragraf. 

Threadul primeste pragraful procesat de worker si dupa incearca sa il salveze in 
memorie intr-un string ce va contine toate paragrafele procesate in ordine 
(am facut acest lucru deoarece ni s-a recomandat sa se scrie o singura data la final, 
pentru a reduce interactiunea cu I/O). 

Aici intervine partea de sincronizare intre cele 4 threaduri din master. Pentru
sincronizare am folosit o varibila globala: crtParagraphNum, care contine
numarul de ordine in fisierul de intrare al paragrafului urmator ce trebuie scris 
in stringul de iesire. Fiecare thread verifica daca numarul paragrafului tocmai procesat
este egal cu acest crtParagraphNum, iar daca este egal inseamna ca paragraful
trebuie scris. Daca nu este egal, inseamna ca mai sunt paragrafe inaintea lui
care nu au fost inca scrise. Fiecare thread va astepta pana cand
crtParagraphNum este egal cu numarul de ordine al paragrafului lor procesat,
iar cand va fi egal, acesta va fi scris la iesire, si crtParagraphNum va fi
incrementat pentru a permite urmatorului paragraf sa fie scris.

Dupa ce a ajuns la finalul fisierului de intrare threadul va trimite un
paragraf de marime zero catre procesul sau worker. Acest paragraf nul este un
semnal pentru procesul worker sa se inchida, deoarece fisierul s-a terminat.

In final procesul master va deschide fisierul de iesire si va scrie stringul
creat de cele 4 threaduri, ce contine toate paragrafele procesate si in
ordine.

Un proces worker creeaza un thread reader, care se ocupa de primirea
paragrafelor de la threadul corespunzator din master. Acest thread reader
primeste paragrafe la infinit pana cand primeste un paragraf de lungime zero.

Cand primeste un paragraf in proceseaza si il trimite inapoi catre master.
Paragraful primit este sub forma de string, cu liniile separate de '\n', de
aceea threadul reader separa paragraful intr-un vector de linii, pe care vor
lucra celelalte P-1 threaduri. Fiecare thread din cele P-1 va procesa un numar
de linii ce este multiplu de 20, cu exceptia unuia, in cazul in care numarul 
de linii nu este multiplu de 20.

Modul in care decid ce linii vor fi procesate de fiecare thread este urmatorul: 
asignez primele 20 de linii primului thread, urmatoarele 20 celui de-al doilea 
si tot asa pana cand ajung la al P-1-lea thread, apoi urmatoarele 20 de linii 
vor fi asignate primului thread, urmatoarele celui de-al doilea thread si tot 
asa pana am asignat toate liniile threadurilor.

Aceste 20 de linii le identific cu numarul de ordine in vector al primei linii
din cele 20. Aceste numere de ordine sunt trimite threadurilor pentru a sti ce
pe ce bucati din vectorul de linii sa lucreze. Threadurile vor incepe de la primu
numar de ordine primit si vor procesa 20 de linii, incluzand-o pe cea de la numarul 
de ordine, dupa care vor trece la urmtoarul numar pana cand le vor procesa
pe toate.

Dupa ce threadurile fac procesarea acestea vor fi inchise, iar vectorul de
linii procesate va fi facut intr-un string ce va fi trimis inapoi threadului
corespunzator din procesul master. Threadurile sunt inchise si redeschise pentru
fiecare paragraf primit.



Rezultatul checkerului local se poate observa mai jos.
Checkerul a fost rulat pe un procesor cu 4 core-uri fiecare core avand 2 threaduri,
in total 8 threaduri.
```
~~~~~~~~~~~~~~~ ESTABLISHING BASE TIME ~~~~~~~~~~~~~~
Test input1.txt took 0.015339374542236328 seconds
Test input2.txt took 0.25429391860961914 seconds
Test input3.txt took 0.6192846298217773 seconds
Test input4.txt took 10.514503955841064 seconds
Test input5.txt took 14.4100661277771 seconds

~~~~~~~~~~~~~~~~~~~ RUNNING TESTS ~~~~~~~~~~~~~~~~~~~
Test input1.txt took 0.36570239067077637 seconds
Test input2.txt took 0.6662261486053467 seconds
Test input3.txt took 1.1178181171417236 seconds
Test input4.txt took 8.239537715911865 seconds
Test input5.txt took 11.406879186630249 seconds

~~~~~~~~~~~~~~~~~~~~~ COMPARING ~~~~~~~~~~~~~~~~~~~~~
Test input1 is correct  SCORE = 2
Test input2 is correct  SCORE = 4
Test input3 is correct  SCORE = 6
Test input4 is correct  SCORE = 8
Test input5 is correct  SCORE = 10

~~~~~~~~~~~~~ DOES IT READ IN PARALLEL? ~~~~~~~~~~~~~
IT DOES
SCORE = 10

~~~~~~~~~~~~~~~~~~~ DOES IT SCALE? ~~~~~~~~~~~~~~~~~~
IT DOES :)
SCORE = 10

Final grade =  10

```
Pentru texte mari (input4 si input5) se poate observa o scadere cu 
aproximativ 20% a timpului de rulare in modul distribuit fata de modul
secvential.
