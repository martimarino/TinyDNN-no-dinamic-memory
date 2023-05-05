TODO
==============================================
- [X] fare in modo che tiny-DNN
	- [X] stampi il numero di training pattern  
	- [X] usi un sottoinsieme dei pattern di training, specificabile come parametro o percentuale
  
- [X] fissare il seed all'inizio, per avere ripetibilità degli esperimenti
- [X] stampare alla fine dell'addestramento la percentuale di corretta classificazione
     su training e su test set
	
- [X] integrare Posit 32 bit con due bit di esponente e rifare l'addestramento
- [X] Serializzazione dei Posit usando la libreria Cereal (da implementare dentro la classe posit) (chiedere a Ruffaldi?)
      - *al momento posit->float->serializzazione e deserializzazione->float->posit*	

- [X] esportare su file la percentuale di corretta classificazione su training e test set
     epoca per epoca, in modo da poter fare un plot in Matlab

- [X] verificare se esiste un costruttore di conversione per tipi unsigned int (posit::posit(unsigned int))

- [X] chiedere a Ruffaldi se fa il fix dell'assegnamento di un float ad un posit

- [~] generalizzare il codice in modo che ammetta il training con un tipo di dato (float_t_tr) 
      e la fase di query con un altro tipo (float_t_qr), che tipicamente avrà un minor numero di bit.
      Stampare a video le prestazioni sul test set ottenute alla fine del training su più bit
      e quelle che si ottengono a quel punto passando ad un minor numero di bit
      - Al momento tenendo separate le due fasi di training e testing, avendo due .cpp distinti
        ho sfruttato la de/serializzazione attraverso il tipo comune float per addestrare la rete con il tipo float, serializzare modello+pesi su file
        , ricaricare il modello nel nuovo programma di testing (compilato con il supporto ai posit) ed eseguire il testing.
      - Per avere entrambi i tipi durante la fase di training ci sarà da riscrivere pesantemente tiny_dnn visto che (per quanto ho trovato fino ad ora) non c'è la templatizzazione di *float_t* in tutte le classi 
        ma un *typedef float_t* definito fin dalla fase di compilazione.

- [X] fare in modo che una rete neurale addestrata con i posit possa essere salvata su file e ricaricata
      da file. Al momento per i float viene usato il metodo serialize. Dobbiamo serializzare (e deserializzare) 
      i posit.
      - Vedi sopra

- [X] Provare il training con Posit8,0 

- [X] Provare ad utilizzare la funzione di attivazione softsign come funzione di attivazione 


- [~] studiare le metriche per il confronto ed implementarle: come si fa a quantificare quanto va meglio
      un posit16 rispetto ad un float16, ad esempio

- [ ] chiedere a Ruffaldi se implementa sqrt and log

- [~] utilizzare posit tabulati invece che posit non tabulati
      - [~] implementare il prodotto passando al logaritmo tabulato, in modo da non dover tabulare ne' il prodotto
            ne' la divisione (questo consentira' di poter usare anche posit12 tabulati, perche' in questo modo 
            non dovrebbe essere invalidata la cache)
      - [ ] caratterizzare dal punto di vista matematico questa soluzione. 
            - [ ] Su quanti bit occorre tabulare il logaritmo?
            - [ ] Aggiungere qualche altro conto interessante, per irrobustire il lato teorico dell'articolo
            
- [ ] drafter l'articolo
- [ ] sottomettere su Springer Journal of Real-Time Image Processing 
      
- [~] aggiungere altri benchmarks di image processing
       - [X] MNIST-fashion [dataset](https://github.com/zalandoresearch/fashion-mnist), [benchmarks](http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/)       
       - [~] CIFAR10 [qua](http://rodrigob.github.io/are_we_there_yet/build/classification_datasets_results.html) FARE Anche il training
       - [~] ImageNet [qua](http://rodrigob.github.io/are_we_there_yet/build/classification_datasets_results.html) FARE Anche il training
       
       - [~] altri benchmark di image processing, possibilmente legati a autonomous driving: ne esistono su internet?
             - https://arxiv.org/pdf/1704.08545.pdf Real time semantic segmentation
             - http://openaccess.thecvf.com/content_cvpr_2016/papers/Cordts_The_Cityscapes_Dataset_CVPR_2016_paper.pdf come sopra, paper sul dataset
             - http://cs231n.stanford.edu/reports/2015/pdfs/lediurfinal.pdf benchmarks sul dataset Stanford Car

- [X] Generalizzare la funzione sigmoid in cppPosit_private in modo che utilizzi la pseudosigmoid (con manipolazione di bit) 
     quando si utilizza un Posit con 0 bit di esponente, mentre utilizzi l'espressione standard in caso contrario
	- [] Correggere la manipolazione di bit in cppPosit

- [ ] trovare il modo di addestrare tinyDNN su GPU (usando il PC del Cococcioni)

- [X] profilare il codice, per indiviare i bottleneck (valgrind + interfaccia grafica Qt)
	- La funzione che viene chiamata più volte e su cui viene perso più tempo è la "forward" del layer di convoluzione

- [ ] documentare il codice ed gli esempi usando doxygen

- [ ] riuscire a far girare le demo dentro jupiter (live test deli esempi)

- [ ] dotarsi di un PC presso il laboratorio di Saponara, con cui collegarsi in remoto al supercomputer di Saponara (100 cores, 128 Gb of RAM)

- [ ] trovare il modo per implementare l'exact dot product (con o senza quires), per migliorare
      ulteriormente l'accuratezza. Le reti neurali coinvolgono sistematicamente i dot-products
      - Dovrei aver individuato dove è attualmente implementato il dot-product da tinyDNN: [qua](https://bitbucket.org/marco_cococcioni/tinydnnalt/src/65ab9512cdc4538f88f0f4f31438f41096d5cf53/tiny_dnn/util/product.h#lines-320)
      NB: forse questa parte conviene approfondirla in futuro e concentrarsi prima sul paper.
      Una volta sottomesso, torneremo sull'exact dot product     


NOTE
==================================================
# Applicazioni *non* safety-critical
La comunita' scientifica attualmente ritiene che servano:
- float a 16 bit per la fase di training
- float a 8 bit per la fase di test

# Applicazioni *safety-critical*
La comunita' scientifica attualmente ritiene che servano:
- float a 32 bit per la fase di training
- float a 16 bit per la fase di test

# Nostri teoremi per applicazioni *safety-critical*
- Il tipo posit14 è sufficiente per la fase di query. Meglio ancora se bastasse un posit12
  Il non-plus ultra sarebbe farsi certificare una rete neurale posit8 per la sola fase di query
  (a quel punto i vantaggi sarebbero incredibili: minore memoria, minore banda, 
   miglior utilizzo dei registri del processore, miglior utilizzo della cache)
- Per la fase di training, posit16 è sufficiente rispetto a float32.
  Addirittura un posit14 potrebbe essere sufficiente per il training.
  A questo punto la cosa si farebbe molto interessante, visto che posit14 si può tabulare.
  Questo avrebbe tanti vantaggi, fra cui la maggior "garantibilità" delle operazioni hardware,
  poichè non si utilizzerebbe nè una FPU nè una PPU ma solo una LUT.

# Prestazioni so far


	https://docs.google.com/document/d/1QAyAU6VI0LsnWQxCdbTwDSOqPA8_C1BUhAWd67NxF0c/edit?usp=sharing


# Vision


===================================================

- Dobbiamo focalizzarci su applicazioni di image processing orientate 
  alla guida assistita/autonoma, in ambiente dunque safety critical.
- Va da se che utilizzeremo prevalentemente reti convolutive.

