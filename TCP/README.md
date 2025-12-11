# Assegnazione 1: Socket TCP 

Progettare ed implementare un’applicazione TCP client/server conforme al seguente protocollo:

1. Il client, invia il messaggio iniziale “Hello” al server. 
2. Ricevuti i dati client, il server visualizza sullo std output un messaggio contenente 
l’indirizzo IP dell’host del client (Esempio: “ricevuti dati dal client con indirizzo: xx”). 
3. Il client legge una stringa di caratteri dallo standard input e la invia al server. 
4. Il server legge la stringa inviata dal client e la visualizza sullo standard output; dopodiché, 
elimina tutte le vocali e la invia nuovamente al client. 
5. Il client legge la risposta inviata dal server e visualizza la risposta sullo standard output, 
dopodiché termina il processo e chiude la connessione 
6. Il server resta in attesa di richieste di connessione da parte di altri client.


# NOTE
Ø La consegna deve avvenire secondo le modalità descritte nella guida che troverete su ADA. 
Non chiedete che sia inviata una conferma di ricezione 
Ø La consegna deve includere il sorgente dei 2 progetti Eclipse (uno per il client e uno per il 
server) come cartelle. Utilizzare la seguente nomenclatura per i due progetti: client
TCP_nomegruppo e server-TCP_nomegruppo 
