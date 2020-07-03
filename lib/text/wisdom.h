#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// I did not find a way to get the length of the array
#define WISDOM_NUMBER_OF_TEXTS          (47)

const String wisdomTexts [] PROGMEM = {
    // Bullshit Bingo
    "Bingo 1: It works on my Machine!",
    "Bingo 2: It worked yesterday!",
    "Bingo 3: It’s never done that before.",
    "Bingo 4: That’s weird.",
    "Bingo 5: Where were you when the program blew up?",
    "Bingo 6: Why do you want to do it that way?",
    "Bingo 7: You can’t use that version on your System!",
    "Bingo 8: Even though it doesn’t work how does it feel?",
    "Bingo 9: Did you check for a Virus on your System?",
    "Bingo 10: Somebody must have changed my code!",
    "Bingo 11: It works, but it hasn’t been tested.",
    "Bingo 12: THIS can’t be the source of that!",
    "Bingo 13: I can’t test everything.",
    "Bingo 14: It’s just some unlucky coincidence.",
    "Bingo 15: You must have the wrong version!",
    "Bingo 16: I haven’t touched that module in weeks.",
    "Bingo 17: There is something funky in your data.",
    "Bingo 18: What did you Type in to get it to crash?",
    "Bingo 19: It must be a hardware problem.",
    "Bingo 20: How is that possible?",
    "Bingo 21: That’s scheduled to be fixed in the next release.",
    "Bingo 22: Yes, we knew that would happen.",
    "Bingo 23: Maybe we just don’t support that platform.",
    "Bingo 24: It’s a Feature we just haven’t updated the specs.",
    "Bingo 25: Surely Nobody is going to use the program like that.",
    // Julia
    "So müde wie heute, war ich bestimmt seit gestern nicht mehr.",
    "Geschenke einpacken. Das rückwärts einparken der Frauen für Männer.",
    "Je größer der Dachschaden desto besser der Blick auf die Sterne.",
    "Ich kann doch nicht alle glücklich machen oder sehe ich aus wie ein Glas Nutella?!",
    "Lass uns lachen, das Leben ist ernst genug.",
    "Ich fühle mich, als könnte ich Bäume ausreissen. Also, kleine Bäume. Vielleicht Bambus. Oder Blumen. Na gut. Gras. Gras geht.",
    "Natürlich mache ich gerne Sport. Deshalb auch so selten. Soll ja schließlich was Besonderes bleiben.",
    "Das Problem will ich nicht, zeig mir das Nächste!",
    "Früher war wirklich alles leichter. Zum Beispiel Ich.",
    "Als Gott mich schuf, grinste er und dachte: Keine Ahnung was passiert, aber lustig wirds bestimmt!",
    "Fahre nie schneller, als dein Schutzengel fliegen kann!",
    "Wer Ordnung hält, ist nur zu faul zum Suchen.",
    "Gebildet ist, wer weiß, wo er findet, was er nicht weiß.",
    "I don't know where I'm going but I'm on my way.",
    "Humor ist versteckte Weisheit.",
    "Schokolade ist Gottes Entschuldigung für Brokkoli.",
    "Wer schwankt hat mehr vom Weg!",
    "Da lernt man Dreisatz und Wahrscheinlichkeitsrechnung und steht trotzdem grübelnd vor dem Backofen, welche der vier Schienen nun die Mittlere ist.",
    "Ich schlafe im Sommer mit offenem Fenster. 1832 Mücken gefällt das.",
    "Was ist ein Keks unter einem Baum? Ein schattiges Plätzchen",
    "Natürlich spreche ich mit mir selbst. Manchmal brauche ich eine kompetente Beratung.",
    "Wie, zu Fuß gehen? Hab doch vier gesunde Reifen!"
};