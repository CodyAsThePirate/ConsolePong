//***********************************************************************************************************//
//  The MIT License (MIT) Copyright                                                 (c) 2013 Simon Neuberger //
//  GR_03_Project_Pong.cpp                            v.1.2                                Datum: 31.12.2010 //
//  ConsolePong.cpp                                   v.1.3                                Datum: 16.10.2013 //
//                                                                                                           //
//                                      Pong - The Mother of all videogames                                  //
//                                                                                                           //
//                                       A project of Simon Neuberger                                        //
//                                              @CodyAsThePirate                                             //
//                                                                                                           //
//  Dieses Spiel ist dem klassischen Pong nachempfunden. Es ist zu zweit zu spielen - Spieler gegen Spieler. //
//  Man die Wahl zwischen zwei Zählweisen: 3 Sätze jeweils bis 11 oder ein einfaches Spiel bis 11. Auch muss //
//  vor Spielbeginn die Schwierigkeitsstufe festgelegt werden, sprich die Ballgeschwindigkeit. Zusätzlich    //
//  besteht die Möglichkeit einer Kurzen Pause, in der Ball aber einfach nur langsamer wird (10000ms). Bei   //
//  Matchsieg eines Spielers wird eine Kurze Gewinnanzeigeseite gezeigt und das Spiel beginnt von vorne mit  //
//  den Auswahlmöglichenkeiten. Sobald der Ball auf Spielrand oder Schläger trifft ertönnt ein Aufprallpong  //
//  -sound sowie bei Sieg ein Jubelsound. (Alle verwendeten Sounds stammen von © Bao Loc Nguyen Van)         //
//                                                                                                           //
//***********************************************************************************************************//

#include <conio.h>
#include <stdio.h>
#include <windows.h>                    // FÜR WinApi FUNKTIONEN
#include <stdlib.h>                     // ENTHÄLT system(...)
#include <process.h>
#pragma comment(lib, "winmm.lib")       // DIESE BIB MUSS GELINKT WERDEN, DAMIT PlaySoundA(...) FUNDKTIONIERT
#include <mmsystem.h>                   // FÜR PlaySoundA(...)

#define YVALUE 21              // MEHRERE MAKROS UM DIE GRÖ?E  ACHTUNG: DER YVALUE MUSS IMMER UNGERADE SEIN! SONST GIBT ES KEINE playingFieldMITTE!
#define XVALUE 46              // DES playingFieldES GANZ SCHNELL
#define XMIDDLE (XVALUE/2)-1   // SCHNELL UND EINFACH NACH
#define YMIDDLE (YVALUE/2)     // BELIEBEN ZU ÄNDERN; ALLES PASST SICH AUTOMATISCH AN!

int playermove(int);
int ballmove(int, int*, int*);
int field(int,char, char*, char*,int,int);
int screen(void);

// MEHR ALS EINE FUNKTION ARBEITET MIT DEM playingField! NATÜRLICH AUCH IN DER MAIN INITIALISIERBAR UND DANN MIT
// EINEM ZEIGER ALS PARAMTER DEN FUNKTIONEN ÜBERGEBEN; JEDOCH WIRD ZAHL DER PARAMETER IRGENDWANN EINFACH UNÜBERSICHTLICH
char playingField[YVALUE][XVALUE]={0};

// HAUPTFUNKTION; MOTOR DES GAMES
int main(void){
	int xBall=0, weiter=0, close=1, control=1, config=1, speed=0, count1=0, count2=0, satz1=0, satz2=0; // GAMESTEUREUNGSVARIABLEN
	int direction=0, upDown=0;              // BALLRICHTUNG STEUERUNGSVARIABLEN
	int *d=&direction, *uD=&upDown;         // UND DEREN ZEIGER; STATT GLOBALE VARIABLEN
	char start, modus='0', linekiller=' ', counter1[6]={"     "}, counter2[6]={"     "};
	screen(); // MUSS NICH SEIN; SIEHT ABER SCHÖNER AUS!!
	
	// DAS GAME
	do{
		
		field(control, modus, counter1, counter2, count1, count2);       // AUSGABE DES GAME AUSGANGSLAGE

		// SPIELEINSTELLUNGEN
		// NUR BEI MATCH-BEGINN ABFRAGE DER SPIELSCHWIERIGKEIT UND DES SPIELMODUS
		if(config == 1){
			config = 0; 
			printf_s("\n\n");
			do{
				printf_s("Difficulty: very easy[1] %c easy[2] %c normal[3] %c hard[4] %c impossible[5] --> ",16,16,16,16); modus=_getche();
				if(modus=='1')speed=150;           // AUSWAHL DER BALLGESCHWINDIGKEIT DURCH DEN SPIELER
				if(modus=='2')speed=80;
				if(modus=='3')speed=50;
				if(modus=='4')speed=23;
				if(modus=='5')speed=0;
				printf_s("\r \r");          // NUR BEI MATCH-BEGINN ABFRAGE DER SPIELSCHWIERIGKEIT UND DES SPIELMODUS// SPRINGT AN ANFANG VON LETZTE ZEILE
			}while (modus!='1' && modus!='2' && modus!='3' && modus!='4' && modus!='5'); // FÄNGT GEDRÜCKTE TASTEN WÄHREND DES SPIELS AB -> ANSONSTEN modus getch() WÜRDE IRGENDWAS EINLESEN
			for(int i=0; i<=80; i++)
				printf_s("\r%*c",i,linekiller);     // LETZTE ZEILE LÖSCHEN	

			do{
				printf_s("Spielmodus: 3 S%ctze bis jeweils 5 [6] %c %c 1 Match bis 11 [7] --> ",132,17,16); modus=_getche();
				if(modus=='6'){count1=-1;count2=-1;}   // AUSWAHL DER ZÄHLWEISE DES SPIELS
				if(modus=='7'){count1=0;count2=0;}
				printf_s("\r \r");
			}while(modus!='6' && modus!='7');
			for(int i=0; i<=80; i++)p
				rintf_s("\r%*c",i,linekiller);     // LETZTE ZEILE LÖSCHEN

			printf_s("Spielstart [s] --> "); 
		}

        start=_getche();   // EINGABE S FÜR START WIRD ABGEWARTET;

		if(start=='s'||start=='S'){
			control=-1; direction=1; upDown=0;  // control REGELT VORGÄNGE IN field() UND playermove(); direction KOORDINIERT DIE BALLRICHTUNGEN in ballmove()

			// SPIELVERLAUF
			// EINE RUNDE LÄUFT SO LANGE BIS DER LINKE ODER RECHTE SPIELRAND ERREICHT WURDE
			for(xBall=XMIDDLE+1; xBall<XVALUE-2 && xBall>3; xBall++){
				Sleep(speed); // DAS MUSS REIN! COMPILER DEBUGGING IST OFT NICHT ORIGINAL SPEED! exe-DATEI OFFENBART DEN WAHREN SPEED!!
				// PAUSE
				if(GetAsyncKeyState(0x42)||weiter==1){
					if(modus=='7')
						printf_s("\n\n\nKURZE Pause!! Ball bewegt sich nur langsamer! Wenn r dann Konzentration! ^^");
					if(modus=='6')
						printf_s("KURZE Pause!! Ball bewegt sich nur langsamer! Wenn r dann Konzentration! ^^");
					weiter=1;Sleep(10000);}
				// PAUSE WIRD AUFGEHOBEN
				if(GetAsyncKeyState(0x52)){
					weiter=0;Sleep(speed);
					if(modus=='7')
						printf_s("\r \r");
						for(int i=0; i<=80; i++)
							printf_s("\r%*c",i,linekiller);} // LöSCHEN DER PAUSEINFO
				// NEUES MATCH
				if(GetAsyncKeyState(0x4E)){
					// ZURÜCKSETZTEN ALLER GAMEVARIABLEN
					for(int i=0; i<=4; i++){
						counter2[i]=' ';
						counter1[i]=' ';
					}
					count1=-1; count2=-1; config=1; satz1=0; satz2=0; modus='0';
					break;
				}
				// GAME WIRD GESCHLOSSEN
				if(GetAsyncKeyState(0x43)){
					close=0;
					break;
				}
				// FüR BALLVERLAUF IN X-RICHTUNG
				if(direction==1||direction==2||direction==3){
					ballmove(xBall, d, uD);      // BEWEGUNG DES BALLES
					playermove(control);         // BEWEGUNG DER SPIELER
					field(control, modus, counter1, counter2, count1, count2);
				}
				
				// FÜR BALLVERLAUF IN -X RICHTUNG
				if(direction==-1||direction==-2||direction==-3){
					xBall-=2;
					ballmove(xBall, d, uD);      // BEWEGUNG DES BALLES
					playermove(control);         // BEWEGUNG DER SPIELER
					field(control, modus, counter1, counter2, count1, count2);
				}
			control=0;
			}

			//SPIELSTÄNDE
			// 3 SÄTZE BIS 5
			if(modus=='6'){
				// SPIELSTAND
				if(xBall>XVALUE-5 && count1<5 && config==0){
					count1++;
					if(count1<4)
						counter1[count1]=1;
				}

				if(xBall<4 && count2<5 && config==0){
					count2++;
					if(count2<4)
						counter2[count2] = 1;
				}
				// field() MUSS AUFGERUFEN WERDEN; DAMIT ES DIE LETZTEN COUNTER-WERTE NOCH MITBEKOMMT BEVOR SIE GELÖSCHT WERDEN!!
				field(control, modus, counter1, counter2, count1, count2);
					if(count1==4)
						satz1++;
					if(count2==4)
						satz2++;
					// BEI SIEG ZURÜCKSETZTEN ALLER WERTE
					for(int i=0; i<=4 ; i++){
						counter2[i]=' ';
						counter1[i]=' ';
					}
					if(satz1==2){
						system("cls");
						printf_s("\n\n\n\n\n\n\n       Spieler 1 hat gewonnen!!!");
						PlaySoundA("winner3.wav",NULL,SND_ASYNC);
						Sleep(5000);
						config=1;
						modus='0';
						satz1=0;
						satz2=0;
					}
					if(satz2==2){
						system("cls");
						printf_s("\n\n\n\n\n\n\n       Spieler 2 hat gewonnen!!!");
						PlaySoundA("winner3.wav",NULL,SND_ASYNC);
						Sleep(5000);
						config=1;
						modus='0';
						satz2=0;
						satz1=0;
					}
					count1=-1;
					count2=-1;
			}

			// 1 MATCH BIS 11
			if(modus=='7'){
				//SPIELSTAND
				if(xBall>XVALUE-5 && count1<11 && config==0)
					count1++;
				if(xBall<4 && count2<11 && config==0)
					count2++;
				// field() MUSS AUFGERUFEN WERDEN; DAMIT ES
				field(control, modus, counter1, counter2, count1, count2);
				if(count1==11||count2==11){
					system("cls");
					// BEI SIEG ZURÜCKSETZTEN ALLER WERTE
					if(count1==11){
						printf_s("\n\n\n\n\n\n\n       Spieler 1 hat gewonnen!!!");
						PlaySoundA("winner3.wav",NULL,SND_ASYNC);
					}
					if(count2==11){
						printf_s("\n\n\n\n\n\n\n       Spieler 2 hat gewonnen!!!");
						PlaySoundA("winner3.wav",NULL,SND_ASYNC);
					}
					Sleep(5000);
					count1=0;count2=0;config=1;
				}
				
			}
		}

		fflush(stdin); 
		xBall=0; 
		control=1;
		system("cls");

	}while(close);

	return 0;
}
			
// ERSTELLT DAS playingField MIT BALL UND SPIELERBALKEN 
int field(int control,char modi, char *count1, char *count2, int one,int two){
	int i, r; 
	static int os1=0, os2=0, os3=0;
	static int ts1=0, ts2=0, ts3=0; 
	static double satz=1;

	COORD fieldDelete;   // VARIABLENTYP COORD AUS DER WINAPI ENTSPRICHT EINEM STRUCT IN C/C++
	fieldDelete.X=18;    // WIRD GEBRAUCHT FÜR DAS "SETZEN DER CORSURPOSITION" DAS SPÄTER IN DIESER FUNKTION ALS BESSERE ALTERNATIVE
	fieldDelete.Y=7;     // ZU system("cls") VERWENDET WIRD

	// DIES DARF NUR EINMAL VOR JEDEM SPIELSTART PASSIEREN!!
	if(GetAsyncKeyState(0x4E) || control==1){
		printf_s("\n                   ~~~ Pong - Die Mutter aller Videospiele ~~~\n");
		printf_s("\n        Men%c: [s] Start [n] Neues Spiel [b/r] KurzePause/Weiter [c] close\n", 129);
		printf_s("              Spieler1 a[hoch]/y[runter] : Spieler2 k[hoch]/m[runter]\n\n");

		// FÜLLEN/AUFSPANNEN DES playingFieldES DURCH LEERZEICHEN
		for(r=0; r<XVALUE-1; r++){
			for(i=0; i<YVALUE; i++){
				playingField[i][r]=' ';
			}
		}
	
		// ERSTELLEN DER MITTELLINIE DES playingFieldES
		for(i=0; i<YVALUE; i++){
			playingField[i][XMIDDLE]='|';
		}

		// ERSTELLEN DER SPIELERPADDLES/BALKEN
		playingField[YMIDDLE-1][2]=17;  //
		playingField[YMIDDLE][2]=17;    // SPIELER 1
		playingField[YMIDDLE+1][2]=17;  //

		playingField[YMIDDLE-1][XVALUE-4]=16;  //
		playingField[YMIDDLE][XVALUE-4]=16;    // SPIELER 2
		playingField[YMIDDLE+1][XVALUE-4]=16;  //

		//ERSTELLT SPIELBALL
        playingField[YMIDDLE][XMIDDLE]=1;

		// ERSTELLEN DES OBEREN SPIELRANDES
		printf_s("                 "); 
		for(i=0; i<=XVALUE; i++){
			printf_s("%c", 254);
		} 
		printf_s("\n");

		// AUSDRUCKEN DES playingFieldES MIT LINKEM UND RECHTEM RAND UND  MIT SPIELERGEBNISANZEIGE
		for(i=0; i<=YVALUE-1 ;i++){
			if(i==YMIDDLE){
				if(modi=='6')
					printf_s("     %s       %c%s%c       %s\n", count1, 254, playingField[YMIDDLE], 254, count2);   // PUNKTESMILEYS BEI SPIEL MIT SÄTZEN
				else if(modi=='7'){
					if((one<10&&two<10)||(one<10&&two>9))
						printf_s("         %d       %c%s%c       %d\n", one, 254, playingField[YMIDDLE], 254, two);     // SPIELSTANDANZEIGE BIS 9
					else if(one>9)
						printf_s("        %d       %c%s%c       %d\n", one, 254, playingField[YMIDDLE], 254, two);     // SPIELSTANDANZEIGE AB 10
					else
						printf_s("                 %c%s%c\n", 254, playingField[YMIDDLE], 254);
				}else 
					printf_s("                 %c%s%c\n", 254, playingField[YMIDDLE], 254);
			}else
			printf_s("                 %c%s%c\n", 254, playingField[i], 254);}
	
		// ERSTELLEN DES UNTEREN SPIELRANDES
		printf_s("                 "); 
		for(i=0; i<=XVALUE; i++){
			printf_s("%c", 254);
		}
	}

	if(control!=1){  // DIES FINDET IMMER WÄHREND DER GAME FOR-SCHLEIFE IN DER MAIN-FUNKTION STATT
		for(i=0; i<=YVALUE-1 ;i++,fieldDelete.Y++){
			if(i==YMIDDLE){
				if(modi=='6')
					printf_s("\n     %s       %c%s%c       %s", count1, 254, playingField[YMIDDLE], 254, count2);   // SPIELSTANDANZEIGE FÜR SÄTZE
				else if(modi=='7'){
					if((one<10&&two<10)||(one<10&&two>9))
						printf_s("\n         %d       %c%s%c       %d", one, 254, playingField[YMIDDLE], 254, two);     // SPIELSTANDANZEIGE BIS 9
					else if(one>9)
						printf_s("\n        %d       %c%s%c       %d", one, 254, playingField[YMIDDLE], 254, two);     // SPIELSTANDANZEIGE AB 10
					else
						printf_s("                  %s \n", playingField[YMIDDLE]);
				}else 
					printf_s("                  %s \n", playingField[YMIDDLE]);
			}else{
				// ALTERNATIVE ZU system("cls"); LÖSCHT NUR DAS playingField UND ÜBERSCHREIBT ES MIT DEM NEUEN; VORTEILE: FLIMMERN IST WEG! PROGRAMM IST SCHNELLER!
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), fieldDelete);
				printf_s("%s", playingField[i]);
			}
		}
		printf_s("\n");
	}

	// FALLS MODUS 6 ANZEIGE EINER ERGEBNISTAFEL
	if(modi=='6'){
		if(satz==1.0){
			if((one==0||one==1||one==2||one==3||one==4) && os1-1<one)
				os1++;
			if((two==0||two==1||two==2||two==3||two==4) && ts1-1<two)
				ts1++;
			if(one==4){
				satz=1.5;
			}else if(two==4){
				satz=1.5;
			}
		}

		if(satz==1.5 && one==-1 && two==-1){
			satz=2.0;
		}

		if(satz==2.0){
			if((one==0||one==1||one==2||one==3||one==4) && os2-1<one)
				os2++;
			if((two==0||two==1||two==2||two==3||two==4) && ts2-1<two)
				ts2++;
			if(one==4){
				satz=2.5;
			}
			else if(two==4){
				satz=2.5;
			}
		}

		if(satz==2.5 && one==-1 && two==-1){
			satz=3.0;
		}

		if(satz==3.0){
			if((one==0||one==1||one==2||one==3||one==4) && os3-1<one)
				os3++;
			if((two==0||two==1||two==2||two==3||two==4) && ts3-1<two)
				ts3++;
			if(one==4){
				satz=0.5;
			}
			else if(two==4){
				satz=0.5;
			}
		}

		if(satz==0.5 && one==-1 && two==-1){
			satz=0;
		}

		printf_s("\n%31c-- 1 --- 2 --- 3 -%c\n",17,16);
		printf_s("%22c Spieler1|  %d  |  %d  |  %d  %c\n",0,os1,os2,os3,16);
		printf_s("%22c Spieler2|  %d  |  %d  |  %d  %c\n",0,ts1,ts2,ts3,16);
		printf_s("%31c------------------%c\n",17,16);
	}

	if(satz==0 || GetAsyncKeyState(0x4E) || (os1==5 && os2==5) || (os1==5 && os3==5) || (os2 && os3==5)
	   || (ts1==5 && ts2==5) || (ts1==5 && ts3==5) || (ts1 && os3==5) || modi=='0'){
		os1=0; os2=0; os3=0;
		ts1=0; ts2=0; ts3=0; 
		satz=1;
	}

	return 0;
}

// SORGT FÜR DIE BEWEGUNG DES BALLES
int ballmove(int Ball,int *direction,int *upDown){
	
	playingField[YMIDDLE][XMIDDLE]=' '; // LÖSCHT STARTBALL IN MITTE MIT LEERZEICHEN

	// ERSTELLEN DER MITTELLINIE MUSS NACH JEDEM DURCHLAUF PASSIEREN; DA BEIM ÜBERQUEREN IMMER GELÖSCHT WIRD
	for(int i=0; i<YVALUE; i++){
		playingField[i][XMIDDLE]='|';
	}

	// ANWEISUNGEN FÜR DIE GRENZSTELLE x=XVALUE-5 -> Schläger2-Ball-Konfrontation
	if(playingField[YMIDDLE+*upDown][Ball+1]==16 && Ball==XVALUE-5){ // WENN BALL-> SCHLÄGER 2
		PlaySoundA("pong.wav",NULL,SND_ASYNC);
		if(playingField[YMIDDLE+*upDown-1][Ball+1]==' '){            // WENN BALL VON VORNE/UNTEN -> SCHLÄGER2 OBERE HÄLFTE
			*direction=-2;Ball--;    // BEDINGUNG FÜR RÜCKWÄRTS-HOCH FOLGEBÄLLE
			playingField[YMIDDLE+*upDown][Ball+1]=' ';               // ÜBERSCHREIBT LETZTEN BALL DER VORHERIGEN BEWEGUNG MIT LEERZEICHEN
		}else
		if(playingField[YMIDDLE+*upDown+1][Ball+1]==' '){           // WENN BALL VON VORNE/OBEN -> SCHLÄGER2 UNTERE HÄLFTE
			*direction=-3; Ball--;  // BEDINGUNG FÜR RÜCKWÄRTS-RUNTER FOLGEBÄLLE
			playingField[YMIDDLE+*upDown][Ball+1]=' ';   // ÜBERSCHREIBT LETZTEN BALL DER VORHERIGEN BEWEGUNG MIT LEERZEICHEN
		}else
		if(playingField[YMIDDLE+*upDown+1][Ball+1]==16 && playingField[YMIDDLE+*upDown-1][Ball+1]==16){ // WENN BALL VON VORNE/UNTEN/OBEN -> SCHLÄGER2 MITTE
			*direction=-1; Ball--; // BEDINGUNG FÜR RÜCKWÄRTS-GERADEAUS FOLGEBÄLLE
			playingField[YMIDDLE+*upDown][Ball+1]=' '; // ÜBERSCHREIBT LETZTEN BALL DER VORHERIGEN BEWEGUNG MIT LEERZEICHEN
		}
	} 

	// ANWEISUNGEN FÜR DIE GRENZSTELLE x=3 -> Schläger1-Ball-Konfrontation
	if(playingField[YMIDDLE+*upDown][Ball-1]==17 && Ball==3){       // WENN BALL -> SCHLÄGER1
		PlaySoundA("pong.wav",NULL,SND_ASYNC);
		if(playingField[YMIDDLE+*upDown-1][Ball-1]==' '){           // WENN BALL VON VORNE/UNTEN -> SCHLÄGER1 OBEN
			*direction=2;	// BEDINGUNG FÜR GERADEAUS-HOCH FOLGEBÄLLE
		}else
		if(playingField[YMIDDLE+*upDown+1][Ball-1]==' '){           // WENN BALL VON VORNE/OBEN -> SCHLÄGER1 UNTEN
			*direction=3;   // BEDINGUNG FÜR GERADEAUS-RUNTER
		}else
		if(playingField[YMIDDLE+*upDown+1][Ball-1]==17 && playingField[YMIDDLE+*upDown-1][Ball-1]==17){ // WENN BALL VON VORNE/UNTEN/OBEN -> SCHLÄGER1 MITTE
			*direction=1; Ball++; // BEDINGUNG FÜR GERADEAUS
			playingField[YMIDDLE+*upDown][Ball-1]=' ';              // ÜBERSCHREIBT LETZTEN BALL DER VORHERIGEN BEWEGUNG MIT LEERZEICHEN
		}
	}

	// FOLGENDE If's SORGEN FÜR ALLE RÜCKWÄRTSBEWEGUNGEN (RÜCKWÄRTS-GERADE;RÜCKWÄRTS-HOCH;RÜCKWÄRTS-RUNTER)
	// RÜCKWARTS-HOCH
	if(*direction==-2){
		*upDown-=1;Ball--;
		playingField[YMIDDLE+*upDown+1][Ball+1]=' ';                // LÖSCHT ALTEN BALL MIT LEERZEICHEN
		playingField[YMIDDLE+*upDown][Ball]=1;                      // SCHREIBT EINEN BALL EINE X/Y STELLE ZURÜCK
        if(*upDown==-YMIDDLE/*playingField[YMIDDLE+upDown][Ball]==playingField[0][Ball]*/){ // FALLS SPIELRAND OBEN
			PlaySoundA("pong.wav",NULL,SND_ASYNC);
			*direction=-3;*upDown-=1;Ball++;
		}
	}

	// RÜCKWÄRTS-RUNTER
	if(*direction==-3){
		*upDown+=1;Ball--;
		playingField[YMIDDLE+*upDown-1][Ball+1]=' ';     // LÖSCHT ALTEN BALL MIT LEERZEICHEN
		playingField[YMIDDLE+*upDown][Ball]=1;           // SCHREIBT EINEN BALL EINE X STELLE ZURÜCK UND EINE Y STELLE VOR
		if(playingField[YMIDDLE+*upDown+1][Ball]==playingField[YVALUE][Ball]){ // FALLS SPIELRAND UNTEN
			PlaySoundA("pong.wav",NULL,SND_ASYNC);
			*direction=-2;
		}
	}

	// RÜCKWÄRTS-GERADEAUS
	if(*direction==-1){
		Ball--;
		playingField[YMIDDLE+*upDown][Ball+1]=' ';   // LÖSCHT ALTEN BALL MIT LEERZEICHEN
		playingField[YMIDDLE+*upDown][Ball]=1;       // SCHREIBT EINEN BALL EINE X STELLE ZURÜCK
	}

	// FOLGENDE if's SORGEN FÜR ALLE VORWÄRTSBEWEGUNGEN (VORWÄRTS-HOCH;VORWÄRTS-RUNTER;FORWÄRTS-GERADE)
	// FORWÄRTS-HOCH
	if(*direction==2){
		*upDown-=1;Ball++;
		playingField[YMIDDLE+*upDown+1][Ball-1]=' ';    // LÖSCHT ALTEN BALL MIT LEERZEICHEN
		playingField[YMIDDLE+*upDown][Ball]=1;          // SCHREIBT EINEN BALL EINE X/Y STELLE VOR
        if(*upDown==-YMIDDLE/*playingField[YMIDDLE+upDown][Ball]==playingField[0][Ball]*/){ // FALLS SPIELRAND OBEN
			PlaySoundA("pong.wav",NULL,SND_ASYNC);
			*direction=3;*upDown-=1;Ball--;
		}
	}

	// FORWÄRTS-RUNTER
	if(*direction==3){
		*upDown+=1;Ball++;
		playingField[YMIDDLE+*upDown-1][Ball-1]=' ';   // LÖSCHT ALTEN BALL MIT LEERZEICHEN
		playingField[YMIDDLE+*upDown][Ball]=1;         // SCHREIBT EINEN BALL EINE X/Y-STELLE VOR
		if(playingField[YMIDDLE+*upDown+1][Ball]==playingField[YVALUE][Ball]){// FALLS SPIELRAND UNTEN
			PlaySoundA("pong.wav",NULL,SND_ASYNC);
			*direction=2;
		}
	}

	// SORGT FÜR DIE GERADEAUSBEWEGUNG BEI START UND SCHLÄGER1-MITTE
	if(*direction==1){
		playingField[YMIDDLE+*upDown][Ball]=1;         // SCHREIBT EINEN NEUEN BALL EINE X-STELLE WEITER
		playingField[YMIDDLE+*upDown][Ball-1]=' ';     // ÜBERSCHREIBT ALTEN BALL MIT LEERZEICHEN
	}
	return 0;
}

// SORGT FÜR DIE BEWEGUNG DER SPIELER
int playermove(int control){
	static int paddle1=0, paddle2=0;

	if(control==-1){paddle1=0;paddle2=0;}    // MÜSSEN VOR JEDEM START AUF NULL GESETZT WERDEN

	if(GetAsyncKeyState(0x41) && playingField[YMIDDLE-1+paddle1][2]!=playingField[0][2]){ // DIESE ZEILE SORGT IMMER DAFÜR DAS BALKEN NICHT ÜBER SPIELRAND KANN
		paddle1--;                                 //   SPIELER 1 NACH OBEN
		playingField[YMIDDLE+2+paddle1][2]=' ';    //   DAS PRINZIP BEI ALLEN VIER RICHTUNGEN IST DASSELBE WIE BEIM BALL 
		playingField[YMIDDLE-1+paddle1][2]=17;     //   MIT ' ' LÖSCHEN->FELDZELLE BESCHREIBEN->LÖSCHEN->BESCHREIBEN...
	}

	if(GetAsyncKeyState(0x59) && playingField[YMIDDLE+2+paddle1][2]!=playingField[YVALUE][2]){
		playingField[YMIDDLE-1+paddle1][2]=' ';   //   SPIELER 1 NACH UNTEN
		playingField[YMIDDLE+2+paddle1][2]=17;
		paddle1++;
	}

	if(GetAsyncKeyState(0x4B) && playingField[YMIDDLE-1+paddle2][XVALUE-4]!=playingField[0][XVALUE-4]){
		paddle2--;     //   SPIELER 2 NACH OBEN
		playingField[YMIDDLE+2+paddle2][XVALUE-4]=' ';
		playingField[YMIDDLE-1+paddle2][XVALUE-4]=16;
	}

	if(GetAsyncKeyState(0x4D) && playingField[YMIDDLE+2+paddle2][XVALUE-4]!=playingField[YVALUE][XVALUE-4]){ //   
		playingField[YMIDDLE-1+paddle2][XVALUE-4]=' ';//    SPIELER 2 NACH UNTEN
		playingField[YMIDDLE+2+paddle2][XVALUE-4]=16;
		paddle2++;
	}

	return 0;
}

// FORMATIERT DAS KONSOLENFENSTER; FÜR SPIELFUNKTIONALITIÄT ABSOLUT ÜBERFLUSSIG; REIN FÜR GESTALTERISCHE ZWECKE
int screen(void){
	// MIT <windows.h> HAT MAN DIE Windows-API ZUR VERFÜGUNG. SEHR NÜTZLICHE FUNKTIONEN! 
	// ABER ACHTUNG: läuft nur auf Windows! Programm verliert Systemunabhängigkeit! BESSER: system() Funktioniert auch für Linux(mit Linuxbefehlen)!

	// ÄNDERT DEN TITEL DER KONSOLE; ALTERNATIVE! AUCH MIT system("title ...")
	SetConsoleTitle(L"Pong - The Mother of all videogames                         by Simon Neuberger @CodyAsThePirate"); 
	// MIT system("") - ENTHALTEN IN <stdlib.h> - KANN MAN BEFEHLE AUS DER WINDOWSKOMMANDOZEILE EINBINDEN!
	system("color E0");  // FARBE DER KONSOLE; SIEHE cmd.exe -> help color FÜR DIE FARBCODES
	system("mode con: lines=35");   // FENSTERGRÖßE DER KONSOLE; Spalten und Zeilen -> mode con: cols= lines=
	return 0;
}



