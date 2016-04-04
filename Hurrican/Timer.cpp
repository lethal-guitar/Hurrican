// Datei : Timer.cpp

// -------------------------------------------------------------------------------------- 
//
// Timer Klasse
// zum Auslesen/Berechnen der Framerate
// und zur Synchronisierung der Bewegungen mit der Framerate
//
// (c) 2002 Jörg M. Winterstein
//
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------------

#include "Globals.h"
#include "Timer.h"
#include "Logdatei.h"
#include "DX8Font.h"

// --------------------------------------------------------------------------------------
// Klassen Funktionen
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Konstruktor, prüft auf den PerformanceCounter und setzt diverse Werte auf den Startwert
// --------------------------------------------------------------------------------------

TimerClass::TimerClass()
	: framesElapsed(0)
	, lastTime(clock.now())
	, SpeedFaktor(1.0f)
{
   maxFPS					= 0;							// best mögliche Peformance fordern
   vergangeneZeit			= 1.0f;
   FPSMinimum				= 10000.0f;						// kleinste Framerate setzen
   FPSMaximum				= 0.0f;							// grösste Framerate setzen
   DurchschnittFramerate	= 0;							// durchscnittliche Framerate
   MoveSpeed				= 10.0f;						// so moven wie bei 60 fps
}

// --------------------------------------------------------------------------------------
// Desktruktor (leer)
// --------------------------------------------------------------------------------------

TimerClass::~TimerClass(void)
{
}

// --------------------------------------------------------------------------------------
// Timer updaten und Zeiten berechnen
// --------------------------------------------------------------------------------------

void  TimerClass::update(void)
{
	using namespace std::chrono;
	++framesElapsed;
	const auto currentTime = clock.now();
	const duration<double> elapsedTime = currentTime - lastTime;

    vergangeneZeit = elapsedTime.count(); // vergangene Zeit neu setzen
    lastTime=currentTime;										// letzten Frame aktualisieren

	aktuelleFramerate=1 / vergangeneZeit;								// Framerate berechnen
	if(aktuelleFramerate>FPSMaximum)								// neue Maximale Framerate ?
		FPSMaximum = aktuelleFramerate;
	if(aktuelleFramerate<FPSMinimum)								// neue Minimale Framerate ?
		FPSMinimum = aktuelleFramerate;

	// Durschnitt der Framerates berechnen
	DurchschnittFramerate = (framesElapsed*DurchschnittFramerate+aktuelleFramerate)
				  		   /(framesElapsed+1);

	// Speedfaktor errechnen
	SpeedFaktor = (float)(MoveSpeed * vergangeneZeit);

	// begrenzen
	if (SpeedFaktor > 1.0f)
		SpeedFaktor = 1.0f;
}

// --------------------------------------------------------------------------------------
// Warten bis die maximal gesetzte Framerate erreicht ist
// --------------------------------------------------------------------------------------

void  TimerClass::wait()
{
	using namespace std::chrono;

	if (maxFPS==0)							// bei Framerate = 0 gleich wieder zurück
		return;								// da wir da nichts abwarten müssen :-)

	int currentFps = 0;
	do
	{
	    duration<double> elapsedSeconds = clock.now() - lastTime;
		currentFps = static_cast<int>(1.0 / elapsedSeconds.count());
	}
	while (currentFps >= maxFPS);
}

// --------------------------------------------------------------------------------------
// Wartetet "Wert" Milli-Sekunden
// --------------------------------------------------------------------------------------

void  TimerClass::wait(int howLong)
{
	using namespace std::chrono;
	while(duration_cast<milliseconds>(clock.now() - lastTime).count() <= howLong);
}

// --------------------------------------------------------------------------------------
// Neue Movespeed setzen, die durch die FPS geteilt wird
// --------------------------------------------------------------------------------------

void TimerClass::SetMoveSpeed(float Wert)
{
	MoveSpeed = Wert;
}

// --------------------------------------------------------------------------------------
// Movespeed zurückliefern
// --------------------------------------------------------------------------------------

float TimerClass::GetMoveSpeed(void)
{
	return MoveSpeed;
}

// --------------------------------------------------------------------------------------
// Maximum Framerate setzen
// --------------------------------------------------------------------------------------

void  TimerClass::SetMaxFPS(int Wert)
{
	maxFPS=Wert;									// Wert setzen
}

// --------------------------------------------------------------------------------------
// vergangene Zeit in Milli-Sekunden holen
// --------------------------------------------------------------------------------------

double TimerClass::getElapsed()
{
	return vergangeneZeit;							// Den Bahnhof in die Luft jagen, was sonst ?
}

// --------------------------------------------------------------------------------------
// Framerate auslesen
// --------------------------------------------------------------------------------------

double TimerClass::getFrameRate(void)
{
	return(aktuelleFramerate);						// Framerate berechnen und zurückgeben
}

// --------------------------------------------------------------------------------------
// Minimale Framerate auslesen
// --------------------------------------------------------------------------------------

double TimerClass::getMinFrameRate(void)
{
	return(FPSMinimum);
}

// --------------------------------------------------------------------------------------
// Maximale Framerate auslesen
// --------------------------------------------------------------------------------------

double TimerClass::getMaxFrameRate(void)
{
	return(FPSMaximum);
}

// --------------------------------------------------------------------------------------
// Durchschnittliche FPS auslesen
// --------------------------------------------------------------------------------------

double TimerClass::getAverageFPS(void)
{
	return DurchschnittFramerate;
}

// --------------------------------------------------------------------------------------
// Wert holen, mit dem die Bewegungen verrechnet werden
// --------------------------------------------------------------------------------------
double TimerClass::getSpeedFactor (void)
{
	return vergangeneZeit;
}

// --------------------------------------------------------------------------------------
// Max und Min FPS resetten
// --------------------------------------------------------------------------------------

void TimerClass::resetMaxMinFPS(void)
{
	FPSMaximum			  = 0.0f;
	FPSMinimum			  = 10000.0f;
	DurchschnittFramerate = aktuelleFramerate;
}

// --------------------------------------------------------------------------------------
// Max und Min und Durchschnitts FPS in Logdatei sichern
// --------------------------------------------------------------------------------------

void TimerClass::WriteLogValues(void)
{
	Protokoll.WriteText("\nMaximale      FPS : ", false); Protokoll.WriteValue((int)(FPSMaximum));
	Protokoll.WriteText(  "Minimale      FPS : ", false); Protokoll.WriteValue((int)(FPSMinimum));
	Protokoll.WriteText(  "Durchschnitts FPS : ", false); Protokoll.WriteValue((int)(DurchschnittFramerate));
}
