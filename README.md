# UNOReverse
Schrijf een programma dat bestaat uit een TCP-server en HTTP-client. De TCP server luistert op poort 22 (default ssh) naar nieuwe verbindingen en achterhaalt het IP-adres van de aanvaller (i.e. bot-net). Hij logt de inlogpoging in een logbestand samen met de ontvangen gegevens, netwerkstatistieken en de geo-locatie m.b.v. IP Geolocation API. Voor dit laatste maakt het programma gebruik van een HTTP-client om de API aan te spreken en de gegevens die terugkomen in de log te bewaren. Verder zal de server een omgekeerde aanval uitvoeren door een grootte hoeveelheid aan data terug te sturen.

## Credits
Ik wil Axel Van Herle uit de 2de BA EA-ICT Bedanken om mij te helpen vlak voor de deadline te helpen om het pynq board als server op te zetten.


