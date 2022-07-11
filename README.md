# Progetto Tiny Farm

Il progetto è stato sviluppato implementato un server in python 
e un client in C per specifiche del progetto.

### Come far girare il progetto?

```shell
make
./collector.py # Viene lanciato il server in python 
./farm file [file ...] [-d file_home_directory] [-n numero_thread] [-q dimensione_buffer] [-t delay_produttore]
```

Una volta che la farm avrà finito il collector verrà chiuso automaticamente dopo aver dato in output sulla console i dati
ricevuti.

### Scelte implementative
È stato aggiunto un ulteriore flag `-d` per andare a definire la home path dei file per poter andare a far girare il programma
su file presenti in un'altra home directory per i file in maniera più semplice. È stato principalmente fatto per testing.<br>
I file del progetto sono stati divisi in più file per facilità di lettura e di manutenibilità del codice.<br>
* <b>funzioni:</b> Contiene funzioni di gestione della farm come ad esempio il calcolo della somma dei long presenti in un file
* <b>produttore-consumatore:</b> Contiene i body del produttore e del consumatore
* <b>structs: </b> Contiene le definizioni delle structs che sono state usate
* <b>testing:</b> Contiene dei file ulteriori di testing e le utilities che sono state fornite inizialmente con il progetto
* <b>xerrori:</b> Contiene i file per la gestione degli errori

È stato scelto di gestire il thread produttore in un thread separato (e non direttamente nel thread del processo principale) principalmente
per poter andare a separare il codice e renderlo più leggibile.<br><br>
Per l'invio dei dati (nome file e somma) dal client (farm) al server (collector) è stato scelto di generare una stringa
(funzione `genera_stringa_socket`) del tipo `nome_file|somma` mediante la `sprintf(3)` che verrà inviata al server e dopo la decodifica andrà
a prendere i due dati mediante una split per poi stamparli sulla console.<br><br>

Ogni file che riceverà il consumatore genererà la propria connessione con il server per inviare i dati che verrà gestita in multithreading
insieme ad altre connessioni.
Alla fine di tutto il thread creerà un' ultima connessione che invierà un segnale (dimensione negativa) al server per notificare
di chiudersi mediante l'invio di una SIGINT al processo stesso.<br><br>

Per poter interrompere il ciclo del consumatore verrà inviato mediante il buffer produttore consumatore un campo con scritto
`BREAK` e per questo motivo non potranno essere analizzati file con questo nome.

