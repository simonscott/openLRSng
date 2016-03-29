#define WATCHDOG_OFF    255
#define WATCHDOG_250MS  1
#define WATCHDOG_500MS  2
#define WATCHDOG_1S     4
#define WATCHDOG_2S     9
#define WATCHDOG_4S     18
#define WATCHDOG_8S     36

// Global variables
volatile int kicksRemaining = 0;
volatile int kicksPerReset = WATCHDOG_OFF;
IntervalTimer wdTimer;
static bool watchdogUsed = false;
bool firstConfig = true;

// Function prototypes
static inline void watchdogReset();
void watchdogConfig(uint8_t x);
void wdTimerISR(void);
void kickTheDog(void);


// Redefine the startup_early_hook which by default disables the watchdog
#ifdef __cplusplus
extern "C" {
#endif
void startup_early_hook()
{
  // Watchdog times out after 256ms.
  SIM_COPC = 0b1000;
}
#ifdef __cplusplus
}
#endif


bool watchdogAvailable()
{
  return true;
}


// Reset the kicksRemaining counter, so that the timer interrupt
// will continue to kick the dog for a while.
void watchdogReset()
{
  noInterrupts();
  kicksRemaining = kicksPerReset;
  interrupts();
}


// Configure how often the watchdog should run
// Watchdog is already configured to run every 250ms. An interrupt runs every 225ms
// to kick the dog. It does this COUNT times.
// The 'timeout' parameter indicates to what value COUNT should initially be set.
//
// This must first be called early on (less than 250ms after power up)
void watchdogConfig(uint8_t timeout)
{
  kicksPerReset = timeout;
  watchdogReset();
  watchdogUsed = 1;

  // If the first time we are configuring the watchdog, also need to setup timer interrupt
  if(firstConfig)
  {
    delay(1);
    kickTheDog();                
    wdTimer.begin(wdTimerISR, 225000);
    firstConfig = false;
  }
}


// This timer interrupt runs every 225ms.
// If there are kicksRemaining, it will kick the dog
void wdTimerISR()
{
  // If we have "disabled" the watchdog, kick it regardless
  if(kicksPerReset == WATCHDOG_OFF)
    kickTheDog();

  // Otherwise only kick the dog if we have kicks remaining
  else if(kicksRemaining > 0)
  {
    kickTheDog();
    kicksRemaining--;    
  }
}

void kickTheDog()
{
  noInterrupts();
  SIM_SRVCOP = 0x55;
  SIM_SRVCOP = 0xAA;
  interrupts();
}

