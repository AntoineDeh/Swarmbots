/**
 * \file  watchdog.c
 * \version  1.1
 * \author Dimitri SOLET
 * \author Louison LEGROS
 * \date Mar 14, 2023
 * \brief Watchdog module for callback functions with timer.
 *
 * \see watchdog.h
 *
 * \section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, Prose A2 2023
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright Prose A2 2023
 *
 */
/* ----------------------  INCLUDES  ---------------------------------------- */
#include "watchdog.h"

#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/**
 * \struct watchdog_t
 * \brief definition of watchdog object.
 */
struct watchdog_t {
    timer_t timer;
    unsigned int  delay;
    watchdog_callback callback;
};
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void watchdog_function(union sigval val)
 * \brief Init a watchdog callback.
 * \author Dimitri SOLET
 *
 * \param val : signal value
 */
static void watchdog_function(union sigval val);
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
watchdog_t * watchdog_create(unsigned int delay, watchdog_callback callback) {
	watchdog_t * this;

	this = (watchdog_t *) malloc(sizeof(watchdog_t));

	this->delay = delay;
	this->callback = callback;

	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = (void *)this;
	event.sigev_notify_function = watchdog_function;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &(this->timer)) != 0) {
		perror("watchdog timer_create");
	}

	return this;
}

void watchdog_start(watchdog_t * watchdog) {
	struct itimerspec itimer;

	int delay_sec = watchdog->delay/1000;
	int delay_nsec = (watchdog->delay - delay_sec*1000)*1000000;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec =  delay_sec;
	itimer.it_value.tv_nsec = delay_nsec;

	if (timer_settime(watchdog->timer, 0, &itimer, NULL) != 0) {
		perror("watchdog timer_settime");
	}
}

void watchdog_cancel(watchdog_t * watchdog) {
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(watchdog->timer, 0, &itimer, NULL) != 0) {
		perror("watchdog timer_settime");
	}
}

void watchdog_destroy(watchdog_t * watchdog) {
	watchdog_cancel(watchdog);

	if (timer_delete(watchdog->timer)) {
		perror("watchdog timer_delete");
	}

	free(watchdog);	
}

/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void watchdog_function(union sigval val) {
    watchdog_t * watchdog = (watchdog_t *)(val.sival_ptr);
    watchdog->callback(watchdog);
}

