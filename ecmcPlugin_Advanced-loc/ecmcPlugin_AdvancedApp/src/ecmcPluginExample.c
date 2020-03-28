/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPluginExample.cpp
*
*  Created on: Mar 21, 2020
*      Author: anderssandstrom
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN
#define ECMC_EXAMPLE_PLUGIN_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif  // ifdef __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecmcPluginDefs.h"
#include "ecmcAdvanced.h"


static int    lastEcmcError  = 0;
static double ecmcSampleRate = 0;
static void*  ecmcAsynPort   = NULL;
static char*  confStr        = NULL;
/** Optional. 
 *  Will be called once after successfull load into ecmc.
 *  Return value other than 0 will be considered error.
 *  configStr can be used for configuration parameters.
 **/
int adv_exampleConstruct(char * configStr)
{
  printf("adv_exampleConstruct with configStr=\"%s\"...\n",configStr);
  confStr = strdup(configStr);
  return 0;
}

/** Optional function.
 *  Will be called once at unload.
 **/
void adv_exampleDestruct(void)
{
  printf("adv_exampleDestruct...\n");
  if(confStr){
    free(confStr);
  }
}

/** Optional function.
 *  Will be called each realtime cycle if definded
 *  ecmcError: Error code of ecmc. Makes it posible for 
 *  this plugin to react on ecmc errors
 *  Return value other than 0 will be considered to be an error code in ecmc.
 **/
int adv_exampleRealtime(int ecmcError)
{
  printf("adv_exampleRealtime...\n");
  //Update asynparam counter
  increaseCounter();
  lastEcmcError = ecmcError;
  return 0;
}

/** Optional function.
 *  Will be called once just before going to realtime mode
 *  Return value other than 0 will be considered error.
 *  ecmcRefs contains pointer to ecmcAsynPort object and ecmc sample rate
 **/
int adv_exampleEnterRT(void* ecmcRefs){
  
  // Determine ecmc sample rate (just for demo)
  ecmcSampleRate = getSampleRate(ecmcRefs);
  printf("Ecmc sample rate is: %lf ms",ecmcSampleRate);

  // Use ecmcAsynPort (just for demo)
  ecmcAsynPort = getAsynPort(ecmcRefs);

  // init asyn param counter
  initAsyn(ecmcAsynPort);
  return 0;
}

/** Optional function.
 *  Will be called once just before leaving realtime mode
 *  Return value other than 0 will be considered error.
 **/
int adv_exampleExitRT(void){
  printf("adv_exampleExitRT...\n");
  return 0;
}

/** Optional plc function 1*/
double adv_customPlcFunc1(double arg1, double arg2)
{
  //printf("adv_customPlcFunc1 %lf, %lf.\n",arg1,arg2);
  return arg1 * arg2;
}

/** Optional plc function 2*/
double adv_customPlcFunc2(double arg1, double arg2, double arg3)
{
  //printf("adv_customPlcFunc2 %lf, %lf, %lf.\n",arg1,arg2,arg3);
  return arg1 * arg2 * arg3;
}

// Register data for plugin so ecmc now what to use
struct ecmcPluginData pluginDataDef = {
  // Name 
  .name = "ecmcExamplePlugin",
  // Description
  .desc = "Advanced example with use of asynport obj.",
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
  // Optional construct func, called once at load. NULL if not definded.
  .constructFnc = adv_exampleConstruct,
  // Optional destruct func, called once at unload. NULL if not definded.
  .destructFnc = adv_exampleDestruct,
  // Optional func that will be called each rt cycle. NULL if not definded.
  .realtimeFnc = adv_exampleRealtime,
  // Optional func that will be called once just before enter realtime mode
  .realtimeEnterFnc = adv_exampleEnterRT,
  // Optional func that will be called once just before exit realtime mode
  .realtimeExitFnc = adv_exampleExitRT,

  // Allow max s custom plc funcs
  .funcs[0] =      
      { /*----customPlcFunc1----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "adv_plugin_func_1",
        // Function description
        .funcDesc = "Multiply arg0 with arg1.",
        /**
        * 11 different prototypes allowed (only doubles since reg in plc).
        * Only one funcArg<argCount> func shall be assigned the rest set to NULL.
        **/
        .funcArg0  = NULL,
        .funcArg1  = NULL,
        .funcArg2  = adv_customPlcFunc1, // Func 1 has 2 args
        .funcArg3  = NULL,
        .funcArg4  = NULL,
        .funcArg5  = NULL,
        .funcArg6  = NULL,
        .funcArg7  = NULL,
        .funcArg8  = NULL,
        .funcArg9  = NULL,
        .funcArg10 = NULL
      },
  .funcs[1] =
      { /*----customPlcFunc2----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "adv_plugin_func_2",
        // Function description
        .funcDesc = "Multiply arg0, arg1 and arg2.",
        /**
        * 11 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        **/
        .funcArg0  = NULL,
        .funcArg1  = NULL,
        .funcArg2  = NULL,
        .funcArg3  = adv_customPlcFunc2, // Func 2 has 3 args
        .funcArg4  = NULL,
        .funcArg5  = NULL,
        .funcArg6  = NULL,
        .funcArg7  = NULL,
        .funcArg8  = NULL,
        .funcArg9  = NULL,
        .funcArg10 = NULL
      },
  .funcs[2] = {0}, // last element set all to zero..

  /** Plugin specific constants (add prefix to not risc collide with 
   *  names from other modules) */
  .consts[0] = {
        .constName = "adv_CONST_1",
        .constDesc = "Test constant \"adv_CONST_1\" = 1.234567890",
        .constValue = 1.234567890,
      },
   .consts[1] = {
        .constName = "adv_CONST_2",
        .constDesc = "Test constant \"adv_CONST_2\" = 9.876543210",
        .constValue = 9.876543210,
      },
  .consts[2] = {0}, // last element set all to zero..
};

ecmc_plugin_register(pluginDataDef);

# ifdef __cplusplus
}
# endif  // ifdef __cplusplus
