# POINTS 19/20
- [x] TCP server luistert op poort 22
- [x] TCP server accepteert meerdere verbindingen
- [x] TCP server achterhaalt IP adres van client
- [x] TCP server logt IP adres in logs
- [x] TCP server start een HTTP client per nieuwe verbinding
- [x] HTTP client maakt een TCP verbinding met IP Geolocation API
- [x] HTTP client stuurt een correcte HTTP GET request
- [x] HTTP client ontvangt de reactie van de HTTP server
- [x] HTTP client extraheert zuiver de volledige json reactie
- [x] HTTP client logt geolocatie in logs
- [x] HTTP client sluit de verbinding zuiver af
- [x] TCP server accepteert gegevens van client en logt ze in logs
- [x] TCP server stuurt zoveel mogelijk gegevens naar de openstaande verbinding
- [x] TCP server houdt een teller bij hoeveel gegevens succesvol zijn afgeleverd en logt deze bij het sluiten van de verbinding in logs
- [x] TCP server sluit de verbinding nadat de client de verbinding sluit
- [x] TCP server kan meerdere verbindingen simultaan verwerken
- [x] Het geheel werkt volledig zonder crashen en er wordt efficiënt met de resources (i.e. memory en sockets) van de computer gebruik gemaakt
- [x] code staat professioneel op GITHUB (i.e. meerdere nuttige commits en uitleg)
- [ ] Het geheel werkt en is beschikbaar op het internet (e.g. thuis m.b.v. NAT of werkt op publieke server)
- [x] mooi extra eens de rest werkt: correcte documentatie a.d.h.v. flowchart, maak gebruik van pthread of IP's worden in een look-up tabel bewaard (e.g. linked-list) om herhalende aanvallen te kunnen loggen

## uitleg pynq z2 als server
![image](https://github.com/JelleClaes2/HTTP-client/assets/114076330/3ddc13ac-6b50-448d-a2ca-219670f71e58)

Ik heb het pynq z2 bord als server opgezet gekregen en mijn code is gecompileerd geraakt. Ik kreeg enkel de a.out niet gerund op het pynq z2 bord. Als dit wel gelukt was was het geheel beschikbaat op het internet.
