#ifdef _WATCHDOG_APP_
#ifndef _WATCHDOG_APP_H_
#define _WATCHDOG_APP_H_

#define WATCHDOG_KEEP_TIMEOUT 5

int watchdog_app_init(void);
int watchdog_app_disable(void);
int watchdog_app_enable(void);
void watchdog_app_close(void);

#endif
#endif
