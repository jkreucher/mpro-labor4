# Labor 4: Ansteuerung einer Rudermaschine

MPRO Labor bei Prof. Dr.‐Ing. Jens Altenburg an der TH-Bingen

## Zielstellung:

Eine Rudermaschine wird mit dem Anschluss SERVO (D6) am Velleman‐Board verbunden. Die Winkelstellung des Ruderhorns hängt von der Impulsbreite am Signaleingang der Steuerelektronik des Servos ab (Vorlesung Timer&ADC). Das Impulssignal wird mit dem PWM‐Modul des Mikrocontrollers erzeugt. Die erforderliche einstellbare Impulsbreite liegt zwischen 1.1 und 2.1 Millisekunden bei einer Periodendauer von ca. 20 Millisekunden.

Diese Impulsbreite wird nun durch Tasteneingaben am Velleman‐Board modifiziert, d.h. das Ruderhorn des Servos folgt den sich ändernden Signal „proportional“. Mit den Tasten SW2 und SW5 wird das Ruderhorn nach rechts bzw. links verstellt. Zur Signalisierung der Verstellbewegung dienen die beiden LEDs ORANGE1/2. Während der Tastenbetätigung leuchten die Signal‐LEDs.

Bitte programmieren Sie die Tasten so, dass beim Dauerdruck das Ruderhorn solange verstellt wird, wie die jeweilige Taste gedrückt bleibt. Wird die Anschlagposition rechts bzw. links erreicht, stoppt die Bewegung. Die angefahrende Position kann auf zwei Speicherplätze abgelegt werden. Speicherplatz 1 wird durch langes Drücken auf SW3 aktiviert. Der lange Tastendruck wird durch aufleuchten der LED RED2 signalisiert. Ein kurzer Tastendruck auf SW3 bewegt das Ruderhorn auf die Speicherstelle. Für die SW4 gilt dies analog. Der Schiebeschalter SW1 kontrolliert die Steuerung der Tasten SW2/SW5. In der einen Position haben die Tasten die oben beschriebene Funktion, in der anderen Stellung ist die Verstellfunktion inaktiv. Die Speichertasten bleiben funktional, lediglich bei inaktiver Verstellung ist eine Neuprogrammierung der Speicherplätze gesperrt.

## Beispiel:
- Mit Tastenbetätigung von SW2 oder SW5 wird die gewünschte Position des Ruderhorns angefahren. Solange SW2 oder SW5 gedrückt sind, leuchtet die LED ORANGE1 oder ORANGE1.

- Ein langer Tastendruck (>= 500) auf die Taste SW3 speichert die Impulszeit in einer Variablen im Programm. Der lange Tastendruck wird mit dem Aufleuchten der LED RED2 quittiert. Beim Loslassen der Taste verlischt RED2.

- Das Ruderhorn wird nun zu einer neuen Position bewegt. Ein kurzer Tastendruck auf SW3 bewegt das Ruderhorn zur zuvor einprogrammierten Stelle. Die LED GREEN2 quittiert die Eingabe.

- Erst wenn durch SW2 oder SW5 diese Position verlassen wird, wird LED GREEN2 wieder ausgeschaltet.

- Mit der Taste SW4 wird ein zweiter Speicherplatz programmiert bzw. angefahren. Das Verhalten entspricht Punkt 2.

## Lösungshinweise:

Die Aufgabe wird in verschiedene Programmfunktionen aufgeteilt:
- Mit den Bewegungstasten SW2/SW5 können „Einzelschritte“ also kleine Positionsänderungen befohlen werden. Als kleinster Schritt wird eine Zeitänderung im PWM‐Signal von 10μs beim einmaligen Tastendruck vorgegeben. Andauernder Tastendruck für zur Dauerbewegung des Ruderhorns. Prüfen Sie ob es sinnvoll ist, die Tasten SW2/SW5 zu entprellen.
- Für die Erzeugung der Servoimpulse nutzen Sie die API PwmOut. Machen Sie sich mit dieser API vertraut. Vor dem Programmieren des Laborversuches prüfen Sie diese API und das Servo mit einem einfachen Testprogramm auf Funktion.
- Vor der eigentlichen Softwareumsetzung überlegen und entscheiden Sie über die unterschiedlichen zeitlichen Abhängigkeiten der Programmfunktionen.
- Kommentieren Sie Ihr Programm sinnvoll und verständlich. Programmieren Sie so, dass Sie bei möglichen Erweiterungen/Ergänzungen Ihre eigene Software verstehen.