# Thread-Pool-Manager
This is a mock-up implementation of a thread manager program. It utilises stack and queue operations in C.

Initializez Pool-ul de threaduri descrescator intr-un for de la N-1 la 0, aloc memorie pentru un thread si ii dau id-ul.
Initializez cele 3 cozi: waiting, running si finished.
Interpretez datele citite din fisier si apoi introduc ordonat in coada de waiting.
Cum fac asta?
Daca lista de waiting este goala, inserez direct elementul apoi intru intr-un while care merge cat timp coada mea are elemente, practic eu imi scot pe rand fiecare element din coada, il compar cu ce vreau eu sa adaug si apoi le mut intr-o coada auxiliara dupa ce am stabilit ordinea si din coada aux inapoi in cea principala. In mare parte am 4 operatii, 2 introduceri in coada aux (elementul scos pentru a compara si elementul nou), o concatenare a cozii vechi peste cea aux, si apoi a cozii aux peste cea veche.
Verific fiecare situatie pe rand: mai intai pentru prioritate mai mare sau timp de executie mai mic sau id mai mic, atunci introduc prima oara elementul nou, apoi cel scos din lista cand compar.
Mai raman cateva situatii astfel. Daca elementele mele de comparat au atat prioritatea cat si timpul de executie identic si cel nou are id-ul mai mare, cat timp relatia asta ramane valabila si coada mea nu e nula(sa zicem ca am 5 elemente asa si vreau sa adaug un al 6-lea) introduc elementele din coada principala in cea aux pana cand asta nu mai e valabil. In punctul acela fie coada originala e goala si adaug la final, fie elementul meu si-a gasit locul si il adaug in coada aux pentru ca dupa el sunt doar elemente mai putin importante, Daca elementul de introdus are fie prioritatea mai mica fie timpul de executie mai mare, "iterez" prin coada, adaugang elementele in coada auxiliara pana cand e valabila oricare din celelalte relatii. 
Pentru a printa o coada sau a arata un task/thread folosesc aceeasi logica de parcurgere a cozilor/stivelor.
