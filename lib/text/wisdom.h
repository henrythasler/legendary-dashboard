#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// Latin-1 character codes
// ä: "\xe4"
// ü: "\xfc"
// ö: "\xf6"
// Ä: "\xc4"
// Ö: "\xd6"
// Ü: "\xdc"
// ß: "\xdf"

const std::vector<String> wisdomTexts PROGMEM = {
    // Bullshit Bingo
    "Bingo 1: It works on my Machine!",
    "Bingo 2: It worked yesterday!",
    "Bingo 3: I've never done that before.",
    "Bingo 4: That's weird.",
    "Bingo 5: Where were you when the program blew up?",
    "Bingo 6: Why do you want to do it that way?",
    "Bingo 7: You can't use that version on your System!",
    "Bingo 8: Even though it doesn't work how does it feel?",
    "Bingo 9: Did you check for a Virus on your System?",
    "Bingo 10: Somebody must have changed my code!",
    "Bingo 11: It works, but it hasn't been tested.",
    "Bingo 12: THIS can't be the source of that!",
    "Bingo 13: I can't test everything.",
    "Bingo 14: It's just some unlucky coincidence.",
    "Bingo 15: You must have the wrong version!",
    "Bingo 16: I haven't touched that module in weeks.",
    "Bingo 17: There is something funky in your data.",
    "Bingo 18: What did you Type in to get it to crash?",
    "Bingo 19: It must be a hardware problem.",
    "Bingo 20: How is that possible?",
    "Bingo 21: That's scheduled to be fixed in the next release.",
    "Bingo 22: Yes, we knew that would happen.",
    "Bingo 23: Maybe we just don't support that platform.",
    "Bingo 24: It's a Feature we just haven't updated the specs.",
    "Bingo 25: Surely Nobody is going to use the program like that.",
    // Julia
    "So m""\xfc""de wie heute, war ich bestimmt seit gestern nicht mehr.",
    "Geschenke einpacken. Das r""\xfc""ckw""\xe4""rts einparken der Frauen f""\xfc""r M""\xe4""nner.",
    "Je gr""\xf6""ßer der Dachschaden desto besser der Blick auf die Sterne.",
    "Ich kann doch nicht alle gl""\xfc""cklich machen oder sehe ich aus wie ein Glas Nutella?!",
    "Lass uns lachen, das Leben ist ernst genug.",
    "Ich f""\xfc""hle mich, als k""\xf6""nnte ich B""\xe4""ume ausreissen. Also, kleine B""\xe4""ume. Vielleicht Bambus. Oder Blumen. Na gut. Gras. Gras geht.",
    "Nat""\xfc""rlich mache ich gerne Sport. Deshalb auch so selten. Soll ja schließlich was Besonderes bleiben.",
    "Das Problem will ich nicht, zeig mir das N""\xe4""chste!",
    "Fr""\xfc""her war wirklich alles leichter. Zum Beispiel Ich.",
    "Als Gott mich schuf, grinste er und dachte: Keine Ahnung was passiert, aber lustig wirds bestimmt!",
    "Fahre nie schneller, als dein Schutzengel fliegen kann!",
    "Wer Ordnung h""\xe4""lt, ist nur zu faul zum Suchen.",
    "Gebildet ist, wer wei""\xdf"", wo er findet, was er nicht wei""\xdf"".",
    "I don't know where I'm going but I'm on my way.",
    "Humor ist versteckte Weisheit.",
    "Schokolade ist Gottes Entschuldigung f""\xfc""r Brokkoli.",
    "Wer schwankt hat mehr vom Weg!",
    "Da lernt man Dreisatz und Wahrscheinlichkeitsrechnung und steht trotzdem gr""\xfc""belnd vor dem Backofen, welche der vier Schienen nun die Mittlere ist.",
    "Ich schlafe im Sommer mit offenem Fenster. 1832 M""\xfc""cken gef""\xe4""llt das.",
    "Was ist ein Keks unter einem Baum? Ein schattiges Pl""\xe4""tzchen",
    "Nat""\xfc""rlich spreche ich mit mir selbst. Manchmal brauche ich eine kompetente Beratung.",
    "Wie, zu Fu""\xdf"" gehen? Hab doch vier gesunde Reifen!",
    // Postillon Newsticker
    "Z""\xe4""hlte bereits 60 Jahre: Amazon-Lagerist bei Inventur verstorben",
    "Mag er roh nie: Mann kocht Nudelgericht",
    "In G""\xe4""nsef""\xfc""""\xdf""chen gesetzt: Praktikant macht es sich bei 'Arbeit' im Schlachthof bequem",
    """\xe4""tsch, Iran: Versteckte Kamera schickt englischen S""\xe4""nger in den nahen Osten",
    "Multibl""\xe4""h-Pers""\xf6""nlichkeit: Psychisch Kranker kann als mehrere Menschen furzen",
    "Nur ein paar Brocken: Englisch-Kenntnisse nicht ausreichend um Arzt den Grad der ""\xfc""belkeit zu erkl""\xe4""ren",
    "Jahrelang Kohlgirl gewesen: Merkel spricht nicht gerne ""\xfc""ber ihre Anfangszeit",
    "Kann ich mal Ihren Stift haben?: Azubi-Leihvertrag steht kurz vor der Unterzeichnung",
    "Ersatzteil aus dem Second-Hand-Laden: Einarmiger Bandit repariert",
    """\xfc""ber Welt tickendes Angebot: Swatch plant kostenlose Riesen-Uhr auf dem Mond",
    "Wieder zu Hause aufgetaucht: H""\xe4""ftling gl""\xfc""ckt Flucht durch Kanalisation",
    "Das Bett h""\xe4""utet ihm sehr viel: Mann h""\xe4""lt trotz unruhiger N""\xe4""chte an Schlafst""\xe4""tte aus Schlangenleder fest",
    "Weltrekordhalter im Kraulen: Schwimmer verw""\xf6""hnt Katze",
    "Opa-Hammer-Gau: Rentner f""\xe4""llt bei Passionsspielen Werkzeug auf Fu""\xdf""",
    "Essenzzeit: Mann nimmt Gericht aus Konzentraten ein",
    "D""\xf6""ner Hebab: Turkish Airlines bietet an Bord nun auch Grillspezialit""\xe4""t an",
    "Na, Geld: Alternder Callboy wird gefragt, warum er ""\xfc""berhaupt noch arbeitet",
    "20 Pfund abgenommen: Dicke Britin hetzt Taschendieb durch die ganze Stadt",
    "Autorennlesung: Schriftsteller rezitiert sein Werk ""\xfc""ber Formel 1 Saison am N""\xfc""rburgring",
    "2G und SD: Veraltetes Handy h""\xe4""ngt in Baum",
    "Beim Bund: General weist Soldaten auf Riss in Hose hin",
    "Pfiff dezent: US Rapper bewies als Schiedsrichter kein Durchsetzungsverm""\xf6""gen", 
    "Tubenarrest: Freche Zahnpasta muss drinnen bleiben",
    "Schlimmen R""\xfc""ckfall erlitten: Alkoholiker konnte nicht wieder stehen",
    "War nicht ohne: Mann von Safer Sex begeistert",
    "Will da auch gar nichts besch""\xf6""nigen: Plastischer Chirurg nimmt Patientin jede Hoffnung",
    "Zwei Dijons: Senfhersteller bringt Ex-Freundin Geschenk", 
    "Helles Lager: Getr""\xe4""nkemarktmitarbeiter findet Bier schneller",
    "Erwischt: Frau beobachtet ihren Mann beim Tindern",
    "Aus dem Bauch heraus: Von Wal verspeister Mann verfasst spontan seine Memoiren",
    "Herdzinsangelegenheit: Kunde m""\xf6""chte Ofen liebend gern auf Raten zahlen",
    "Rotes Kreuz: Rettungssanit""\xe4""ter klagt ""\xfc""ber R""\xfc""ckenschmerzen nach S/M-Wochenende",
    "Tarte ""\xfc"", Tarte A: B""\xe4""cker wegen verr""\xfc""ckter Nummerierung in Psychiatrie eingeliefert",
    "Muss neuen aufsetzen: Mann sch""\xfc""ttet sich Kaffee ""\xfc""ber den Hut",
    "Springer Steve L.: Neonazi trug beim Suizid seine Lieblingsschuhe",
    "Hintergrund unklar: Maler zerst""\xf6""rt Stillleben",
    "Im Karsten: Schwulenporno endlich fertig",
    "Nur eine Kugel: Revolverheld erschie""\xdf""t knausrigen Eisdielenkunden",
    "War noch gr""\xfc""n hinter Tenoren: Fahranf""\xe4""nger rast auf Kreuzung in Tourbus-Heck",
    "Speichermedium: Auf Dachboden lebende Frau mit ""\xfc""bernat""\xfc""rlichen F""\xe4""higkeiten findet USB-Stick",
    "Thomas' Scotch-Alk: Whisky von Showmaster entwendet",
    "Alles au""\xdf""er Kontrolle: T""\xfc""rsteher nahm Job nicht ernst",
    "Mittags Sch""\xe4""fchen gehalten: Hirte macht Pause mit Lamm im Arm",
    "Tausend Zsa Zsa: Trickreicher Erfinder klont mehrfach Hollywoodlegende",
    "Sitzt gut: H""\xe4""ftling mit Kleidung und Schemel zufrieden",
    "S""\xe4""uchenschutzgesetz: Kleine Schweine m""\xfc""ssen bei Pandemie als erstes gerettet werden",
    "Beh""\xe4""lter f""\xfc""r sich: Betrunkener erkl""\xe4""rt auf Party nicht, warum er Eimer dabei hat",
    "Rasch aua: Autobahn""\xfc""berquerung zu Fuß endet im Krankenhaus",
    "Da kennt er nix: Vollidiot meldet sich zu Astrophysik-Quiz an",
    "Social Diss-Dancing: Waldorfsch""\xfc""ler postet beleidigende Videos auf Facebook",
    "Der totale Kn""\xfc""ller: Kopierer bricht Verkaufsrekorde trotz schlechten Papiereinzugs",
    "Qual-Lied-Hetz-Management: Nach ISO9001 ist es erlaubt, ""\xfc""ber 'Atemlos durch die Nacht' herzuziehen",
    "Steckt in Apache: Winnetou in flagranti mit Liebhaber erwischt",
    "Streit ""\xfc""ber fehlende Stra""\xdf""e eskaliert: B""\xfc""rgermeister schickt Erschlie""\xdf""ungskommando",
    "Schwamm: SpongeBob besteht Seepferdchen",
    "F""\xfc""hrt ein Schattendasein: Von Hand auf Leinwand projiziertes Krokodil",
    "Ernie Dirigent: Bert erlebt Berufung von Freund zum Orchesterleiter als Dem""\xfc""tigung",
    "Wegen Kontaktsperre: Batterie verweigert Dienst",
    "Nach dem Stunt teert Inge: Filmdouble muss voraussichtlich Zweitjob im Stra""\xdf""enbau annehmen",
    "Geh""\xf6""ren zur Riesigkotgruppe: Dicke leiden unter Klopapiermangel",
    "Bekam einen R""\xfc""ffel: Zahnloser muss sich zur Strafe als Elefant verkleiden",
    "Hahn-Twerker: Zimmermann macht Potanz-Video im Gockel-Kost""\xfc""m",
    "Unterschiedliche Vorstellungen: Kinobesitzerehepaar trennt sich",
    "Siel bereisen: Florian macht Deichtour",
    "Macht sich nichts vor: Pfleger wei""\xdf"" eigentlich, dass Ansteckungsrisiko ohne Mundschutz gr""\xf6""""\xdf""er ist",
    "K. Russell: Schauspieler nutzt Fahrgesch""\xe4""ft",
    "Zieht Schlussstrich: Mann trennt sich von Lieblingsstift",
    "Prinzessbohnen: K""\xf6""nig r""\xe4""t seinem Sohn zu mehr Gem""\xfc""se",
    "K""\xf6""nnte jeden treffen: Mann immun gegen Coronavirus",
    "In Skiflagge geraten: Insolventer Quizteilnehmer tr""\xe4""gt Riesenslalomzubeh""\xf6""r als Schal",
    "Wegen Best""\xe4""ubungsmitteln: Biene verhaftet",
    "Eilegeboten: Kundschafter mahnen H""\xfc""hner zu rechtzeitiger Steigerung der Produktion bis Ostern",
    "Verein-Zelt: Sch""\xfc""tzenfest-Pavillon darf nur noch von je einem Mitglied betreten werden",
    "Stoff vers""\xe4""umt: N""\xe4""herin muss nach Feierabend noch f""\xfc""r Kurs lernen",
    "Stand dort: Wanderer sah auf Umgebungsplan, wo er sich befand",
    "In Betrieb genommen: Sexroboter vor Auslieferung getestet",
    "Wertpapiere: Banker investiert in Toilettenrollen",
    "Musste dem Rechnung tragen: Frau akzeptiert, dass Freund nach Leistenbruch unm""\xf6""glich Kassenzettel heben kann",
    "Hat endlich eine Rolle ergattert: Erfolgloser Schauspieler bringt Klopapier nach Hause",
    "Stand kurz vor einer L""\xf6""sung: Impfstoffentwickler atmet hochgiftige D""\xe4""mpfe ein",
    "S. Raab bellt im Karton: Entertainer hat den Verstand verloren",
    "F""\xe4""llt leicht: Neymar auch in Verkleidung unschwer zu erkennen",
    "Total vervirt: Coronapatient l""\xe4""uft orientierungslos durch Fu""\xdf""g""\xe4""ngerzone",
    "'Wang, w""\xfc""rz mal Widder...richtig so...mehr!': Carrell empfand Hammelfleisch am China-Imbiss als zu fad",
    "Brachte nur noch Vieren nach Hause: Familie von schlechtem Sch""\xfc""ler in Quarant""\xe4""ne",
    "Dia mit Lungen aufgenommen: Polizei jagt Organfotografen",
    "Einrichtungshaus: IKEA mahnt Kunden zur Einhaltung der Laufroute",
    "Trampolin: Warschauerin nimmt H""\xfc""pfger""\xe4""t mit in die Stra""\xdf""enbahn",
    "Mustafa werden: Muslima will nach Geschlechtsumwandlung dominanter auftreten",
    "Das wird sich noch zeigen: Murmeltierexperte streitet mit entt""\xe4""uschten Touristen ""\xfc""ber seine Fachkompetenz",
    "'Husten! Wir haben ein Problem.': ISS meldet ersten Corona-Fall",
    "Macht jetzt in Antiquit""\xe4""ten: Vasenh""\xe4""ndler findet kein Klo",
    "F""\xfc""hrt Ananas-Serum: Arzt t""\xe4""uscht Patienten mit Impfstoff aus Fruchtsaft",
    "Immer nur Fish &amp;amp; Ships: Godzilla findet englische K""\xfc""che ""\xf6""de",
    "h.c. nicht alle: Uni-Rektor stellt fast nur Dozenten mit Ehrendoktorw""\xfc""rde ein",
    "Waren verdorben: Ordin""\xe4""re Marktfrauen boten faules Obst an",
    "Eine einzige Schikane: Formel-1-Fahrer ""\xfc""ber schlechten Kurs emp""\xf6""rt",
    "File geboten: Datei zu verkaufen",
    "Schnappt nach Luft: Hund dreht in Sauna durch",
    "'Kiek, Erik, ih!': Berlinerin zeigt ihrem Freund toten Hahn",
    "Nicht auszumalen: Kinderbuch mit 10.000 Seiten ver""\xf6""ffentlicht",
    "Schlafend gestellt: Einbrecher lag bei Zugriff noch im Bett",
    "finnish.net: Homepage in Helsinki f""\xfc""r Hessen unm""\xf6""glich zu erreichen",
    "Arbeitet fieberhaft: Immunsystem l""\xe4""uft auf Hochtouren",
    "Petryschale: Frauke z""\xfc""chtet Halsw""\xe4""rmer im Labor",
    "Hinter Heer ist man schlauer: Soldat will nicht an die Front",
    "Fellig losgel""\xf6""st: Major Tom startete unrasiert",
    "'Sch""\xfc""tt Senf herein!': Jugendlicher stiftet Kumpel an, Schie""\xdf""sport-Club einen Streich zu spielen",
    "Leckt ein bisschen: Chemiker untersucht unbekannte Substanz unter Silo",
    "Dieb rennt: Fl""\xfc""chtiger Taschenr""\xe4""uber z""\xfc""ndete Opfer an",
    "Muss zum K""\xe4""ferorthop""\xe4""den: Insekt hat ""\xfc""berbiss",
    "Punk kaut Tomaten: Linker Veganer hebt mehrfach Geld ab",
    "Spricht mit gespaltener Zunge: Indianer  l""\xfc""gt ""\xfc""ber Unfall mit Jagdmesser",
    "Wiesloch: Frau aus Baden-W""\xfc""rttemberg s""\xe4""uft",
    "Patienten m""\xfc""ssen mit gr""\xf6""""\xdf""eren Einschnitten rechnen: Chirurgiepraxis spart an feinen Skalpellen",
    "Ein sch""\xf6""ner Zug von ihm: Modell-Eisenbahner verschenkt sein liebstes Exemplar",
    "Da gibts Gedr""\xe4""nge: Hohes Menschenaufkommen bei hessischem Spirituosenh""\xe4""ndler",
    "Arsch reckend: Immer mehr M""\xe4""nner gehen in Yoga-Kurs",
    "PDF: Geheimes Dokument offenbart verkehrte Politik der FDP",
    "H""\xe4""ufig in Elefantenporno zu sehen: Sto""\xdf""szene",
    "Ging ihm dann auch direkt auf: Mann h""\xe4""tte Hosenrei""\xdf""verschluss vor Vorstellungsgespr""\xe4""ch besser repariert",
    "Ein einmaliges Erlebnis:  Marodes Bungeeunternehmen versucht es mit ehrlicher Werbung", 
    "Gold richtig: Quizshow-Teilnehmer beantwortet Frage nach Edelmetall", 
    "Ein Fach nur, d""\xe4""mlich: Merkw""\xfc""rdiger Apothekerschrank floppt", 
    "Die wichtigsten Wokgabeln: Asiatischer Koch paukt f""\xfc""r Pr""\xfc""fung in Besteckkunde", 
    "Rote Arme Fraktion: Andreas Baader und Ulrike Meinhof haben Sonnenbrand", 
    "Hat ihn dumm angemacht: Vegetarier von Salat verpr""\xfc""gelt", 
    "Er ist jetzt an einem besseren Ort: Journalist springt aus dem Fenster der BILD-Redaktion",
    "War als erster an der Ziehlinie: Junkie gewinnt Wettrennen um Kokain",
    "Aphte: Nazi wegen Mundh""\xf6""hlenentz""\xfc""ndung unf""\xe4""hig, seine Lieblingspartei zu nennen",
    "Kann ein Liter Fond singen: Mann wei""\xdf"", wie es ist, beim Tr""\xe4""llern Bratensaft zu produzieren",
    "War eiskalt vor dem Tor: Platzwart findet erfrorenen St""\xfc""rmer",
    "Will nicht, dass jeder seinen Schwan zieht: Bekleideter Mann bindet Wasservogel an",
    "Ein Schlag ins Gesicht: Masochist von entt""\xe4""uschendem Domina-Angebot gekr""\xe4""nkt",
    "Serge ade: Bayernspieler muss trotz hervorragender Leistungen den Verein verlassen",
    """\xfc""ber Alf lecken: Masernpatient versucht Krankheit durch Lutschen an Au""\xdf""erirdischem zu lindern",
    "Wie die Lehm-Inge: Kinder beim T""\xf6""pfern von Klippe gesprungen",
    "'""\xe4""hm, Sie Hemmer!': Rapper als Spa""\xdf""bremse beschimpft",
    "Wageneber im Kofferraum: Franzose mit Reifenpanne hatte Schwein",
    "Steiffhundfest: Kind behauptet, sein Stofftier habe Geburtstag",
    "War am saugen: Ehefrau ""\xfc""berrascht ihren Mann mit neuer Haushaltshilfe",
    "Gingen um die Welt: Bilder von Extremdistanz-Wanderern in allen Nachrichten pr""\xe4""sent",
    "Bekommt lecker Li: Kampfhund wird mit Chinesen belohnt",
    "Ohne Witz: Mario Barth ist nicht lustig",
    "Freut sich auf 'at home-Pils': Nuklearphysiker hat Feierabend",
    "Zweibr""\xfc""cken: Zahn""\xe4""rztin aus der Pfalz erlebt ruhigen Vormittag",
    "Irre summen: Anwohner m""\xfc""ssen f""\xfc""r L""\xe4""rmschutzwand vor Psychiatrie zahlen",
    "H""\xf6""ren sich gegenseitig ab: Spionage-Azubis lernen f""\xfc""r ihre Abschlusspr""\xfc""fung",
    "Kaviar im Zeugnis: Bayerischer Lehrer belohnt rogens""\xfc""chtigen Sch""\xfc""ler",
    "Genfer: Zahnloser Schweizer gibt sich f""\xfc""r ehemaligen deutschen Au""\xdf""enminister aus",
    "Lange nicht Font""\xe4""nen geh""\xf6""rt: Meeresanwohner wundert sich, wo Wale geblieben sind"
	// Murphys law
	"If anything can go wrong, it will.",
	"If anything just cannot go wrong, it will anyway.",
	"If everything seems to be going well, you have obviously overlooked something",
	"Smile . . . tomorrow will be worse.",
	"Matter will be damaged in direct proportion to its value.",
	"Research supports a specific theory depending on the amount of funds dedicated to it.",
	"In nature, nothing is ever right. Therefore, if everything is going right ... something is wrong.",
	"It is impossible to make anything foolproof because fools are so ingenious.",
	"Everything takes longer than you think.",
	"Every solution breeds new problems.",
	"The legibility of a copy is inversely proportional to its importance.",
	"A falling object will always land where it can do the most damage.",
	"A shatterproof object will always fall on the only surface hard enough to crack or break it.",
	"The other line always moves faster.",
	"In order to get a personal loan, you must first prove you don't need it.",
	"Anything you try to fix will take longer and cost you more than you thought.",
	"Build a system that even a fool can use, and only a fool will use it.",
	"Everyone has a scheme for getting rich that will not work.",
	"In any hierarchy, each individual rises to his own level of incompetence, and then remains there.",
	"There's never time to do it right, but there's always time to do it over.",
	"When in doubt, mumble. When in trouble, delegate.",
	"Anything good in life is either illegal, immoral or fattening.",
	"Murphy's golden rule: whoever has the gold makes the rules.",
	"A Smith & Wesson beats four aces.",
	"In case of doubt, make it sound convincing.",
	"Never argue with a fool, people might not know the difference.",
	"Whatever hits the fan will not be evenly distributed.",
	"Where patience fails, force prevails.",
	"Just when you think things cannot get any worse, they will.",
	"Great ideas are never remembered and dumb statements are never forgotten.",
	"When you see light at the end of the tunnel, the tunnel will cave in.",
	"Traffic is inversely proportional to how late you are, or are going to be.",
	"The probability of being observed is in direct proportion to the stupidity of ones actions.",
	"Those who know the least will always know it the loudest.",
	"Just because you CAN do something doesn't mean you SHOULD.",
	"Chaos always wins, because it's better organized.",
	"If at first you don't succeed destroy all evidence that you ever tried.",
	"It takes forever to learn the rules and once you've learned them they change again",
	"You will find an easy way to do it, after you've finished doing it.",
	"Anyone who isn't paranoid simply isn't paying attention.",
	"A valuable falling in a hard to reach place will be exactly at the distance of the tip of your fingers.",
	"The probability of rain is inversely proportional to the size of the umbrella you carry around with you all day.",
	"Nothing is impossible for the man who doesn't have to do it himself.",
	"The likelihood of something happening is in inverse proportion to the desirability of it happening.",
	"Common Sense Is Not So Common.",
	"Power Is Taken... Not Given.",
	"Two wrongs don't make a right. It usually takes three or four.",
	"The difference between Stupidity and Genius is that Genius has its limits.",
	"Those who don't take decisions never make mistakes.",
	"Anything that seems right, is putting you into a false sense of security.",
	"Its never so bad it couldn't be worse.",
	"When saying that things can not possibly get any worse - they will.",
	"The person ahead of you in the queue, will have the most complex transaction possible.",
	"Logic is a systematic method of coming to the wrong conclusion with confidence.",
	"Technology is dominated by those who manage what they do not understand.",
	"The opulence of the front office decor varies inversely with the fundamental solvency of the firm.",
	"The attention span of a computer is only as long as it electrical cord.",
	"Nothing ever gets built on schedule or within budget.",
	"The first myth of management is that it exists.",
	"A failure will not appear till a unit has passed final inspection.",
	"To err is human, but to really foul things up requires a computer.",
	"Any sufficiently advanced technology is indistinguishable from magic.",
	"Nothing motivates a man more than to see his boss putting in an honest day's work.",
	"Some people manage by the book, even though they don't know who wrote the book or even what book.",
	"To spot the expert, pick the one who predicts the job will take the longest and cost the most.",
	"After all is said and done, a hell of a lot more is said than done.",
	"The only perfect science is hind-sight.",
	"If an experiment works, something has gone wrong.",
	"When all else fails, read the instructions.",
	"The degree of technical competence is inversely proportional to the level of management.",
	"The remaining work to finish in order to reach your goal increases as the deadline approaches.",
	"It is never wise to let a piece of electronic equipment know that you are in a hurry.",
	"If you are not thoroughly confused, you have not been thoroughly informed.",
	"Standard parts are not.",
	"The bolt that is in the most awkward place will always be the one with the tightest thread.",
	"In today's fast-moving tech environment, it is a requirement that we forget more than we learn.",
	"It is simple to make something complex, and complex to make it simple.",
	"If it works in theory, it won't work in practice. If it works in practice it won't work in theory.",
	"Any tool dropped will fall where it can cause the most damage.",
	"When you finally update to a new technology, is when everyone stop supporting it.",
	"If you think you understand science (or computers or women), you're clearly not an expert",
	"Technicians are the only ones that don't trust technology.",
	"The repairman will have never seen a model quite like yours before.",
	"In theory there is no difference between theory and practice, but in practice there is.",
	"Never underestimate incompetency.",
	"Any given program, when running, is obsolete.",
	"Any given program costs more and takes longer each time it is run.",
	"If a program is useful, it will have to be changed.",
	"If a program is useless, it will have to be documented.",
	"Any given program will expand to fill all the available memory.",
	"Program complexity grows until it exceeds the capability of the programmer who must maintain it.",
	"Adding manpower to a late software project makes it later.",
	"No matter how many resources you have, it is never enough.",
	"If a program has not crashed yet, it is waiting for a critical moment before it crashes.",
	"Software bugs are impossible to detect by anybody except the end user.",
	"A failure in a device will never appear until it has passed final inspection.",
	"An expert is someone brought in at the last minute to share the blame.",
	"A patch is a piece of software which replaces old bugs with new bugs.",
	"The chances of a program doing what it's supposed to do is inversely proportional to the number of lines of code used.",
	"The longer it takes to download a program the more likely it won't run.",
	"It's not a bug, it's an undocumented feature.",
	"Bugs mysteriously appear when you say, Watch this!",
	"The probability of bugs appearing is directly proportional to the number and importance of people watching.",
	"If a project is completed on schedule, it wasn't debugged properly.",
	"If it works, it's production. If it doesn't, it's a test.",
	"Real programmers don't comment their code. If it was hard to write, it should be hard to understand.",
	"A computer that has been on the market for 6 weeks is still usable as a boat anchor.",
	"Computers let you waste time efficiently.",
	"A pat on the back is only a few inches from a kick in the pants.",
	"Don't be irreplaceable, if you can't be replaced, you can't be promoted.",
	"It doesn't matter what you do, it only matters what you say you've done and what you say you're going to do.",
	"The more crap you put up with, the more crap you are going to get.",
	"When the bosses talk about improving productivity, they are never talking about themselves.",
	"Mother said there would be days like this, but she never said there would be so many.",
	"--> The last person that quit or was fired will be the one held responsible for everything that goes wrong. :-)",
	"If it wasn't for the last minute, nothing would get done.",
	"The longer the title, the less important the job.",
	"If you have a little extra money to blow, something will break, and cost more than that little extra.",
	"If the salesperson says, All you have to do is..., you know you're in trouble.",
	// general
	"When I was a kid my parents moved a lot, but I always found them.",
	"Life is short. Smile while you still have teeth.",
	"The best way to teach your kids about taxes is by eating 30 percent of their ice cream.",
	"I'm writing a book. I've got the page numbers done.",
	"I have always wanted to be somebody, but I see now I should have been more specific.",
	"Knowledge is like underwear. It is useful to have it, but not necessary to show it off.",
	"If a book about failures doesn't sell, is it a success?",
	"A lie gets halfway around the world before the truth has a chance to get its pants on.",
	"It's okay if you don't like me. Not everyone has good taste.",
	"Everything is changing. People are taking the comedians seriously and the politicians as a joke.",
	"I like long walks, especially when they are taken by people who annoy me.",
	"The only way to keep your health is to eat what you don't want, drink what you don't like, and do what you'd rather not.",
	"When nothing is going right, go left.",
	"Never miss a good chance to shut up.",
	"I'd like to live like a poor man – only with lots of money.",
	"My life feels like a test I didn't study for.",
	"I don't go crazy. I am crazy. I just go normal from time to time.",
	"My bed is a magical place where I suddenly remember everything I forgot to do.",
	"I'm actually not funny. I'm just really mean and people think I'm joking.",
	"Sometimes I want to go back in time and punch myself in the face.",
	"If you are lonely, dim all lights and put on a horror movie. After a while it won't feel like you are alone anymore.",
	"Fries or salad? sums up every adult decision you have to make.",
	"If we're not meant to have midnight snacks, why is there a light in the fridge.",
	"My doctor told me to watch my drinking. Now I drink in front of a mirror.",
	"I drive way too fast to worry about cholesterol.",
	"As your best friend I'll always pick you up when you fall, after I finish laughing.",
	"Some people are like clouds. When they disappear, it's a beautiful day.",
	"I'm not arguing. I'm simply explaining why I'm right.",
	"No wonder the teacher knows so much; she has the book.",
	"The most ineffective workers are systematically moved to the place where they can do the least damage: management.",
	"The best way to appreciate your job is to imagine yourself without one.",
	"If I had asked people what they wanted, they would have said faster horses. Henry Ford",
	"The closest a person ever comes to perfection is when he fills out a job application form.",
	"Don't yell at your kids! Lean in real close and whisper, it's much scarier.",
	"The quickest way for a parent to get a child's attention is to sit down and look comfortable.",
	"I don't know what's more exhausting about parenting: the getting up early, or the acting like you know what you're doing.",
	"It just occurred to me that the majority of my diet is made up of the foods that my kid didn't finish.",
	"When your children are teenagers, it's important to have a dog so that someone in the house is happy to see you.",
	"I never know what to say when people ask me what my hobbies are. I mean, I'm a dad.",
	"My wife and I were happy for twenty years. Then we met.",
	"Marriage... it's not a word, it's a sentence.",
	"Marry a man your own age; as your beauty fades, so will his eyesight.",
	"Marriage is the triumph of imagination over intelligence. Second marriage is the triumph of hope over experience.",
	"The most terrifying thing any woman can say to me is Notice anything different?",
	"When a man brings his wife flowers for no reason, there's a reason."
};