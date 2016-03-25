static inline void watchdogReset();
#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (1)
#define WATCHDOG_32MS   (1)
#define WATCHDOG_64MS   (1)
#define WATCHDOG_125MS  (1)
#define WATCHDOG_250MS  (1)
#define WATCHDOG_500MS  (1)
#define WATCHDOG_1S     (1)
#define WATCHDOG_2S     (1)
#define WATCHDOG_4S     (1)
#define WATCHDOG_8S     (1)
void watchdogConfig(uint8_t x);
static bool watchdogUsed = false;

bool watchdogAvailable()
{
  return true;
}

void watchdogReset()
{
}

void watchdogConfig(uint8_t x)
{
  if (watchdogAvailable()) {
  }
}

