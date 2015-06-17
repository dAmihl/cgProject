/* 
 * File:   DeltaTime.h
 * Author: dAmihl
 *
 * Created on 17. Juni 2015, 15:49
 */

#ifndef DELTATIME_H
#define	DELTATIME_H

/* variables for computing elapsed time since last render */
extern int deltaTime;
extern int oldTimeSinceStart;

void computeDeltaTime();



#endif	/* DELTATIME_H */

