/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcAdvanced.cpp
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*      Credits to  https://github.com/sgreg/dynamic-loading 
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN

// Error Codes
#define ECMC_ERROR_ASYNPORT_NULL 1
#define ECMC_ERROR_ASYN_PARAM_FAIL 2

#include "ecmcPluginDataRefs.h"
#include "ecmcAdvanced.h"

// Vars
static int counter = 0;
static ecmcAsynDataItem *paramCount = NULL;

// get ecmc rt sample rate from ecmcRefs
double getSampleRate(void* ecmcRefs) {
  if(ecmcRefs) {
    ecmcPluginDataRefs* dataFromEcmc = (ecmcPluginDataRefs*)ecmcRefs;
    // call report()!    
    return dataFromEcmc->sampleTimeMS;
  }
  return -1;
}

// Demo simple use of asynPort. Most ecmc data can be accessed from asynPort 
void* getAsynPort(void* ecmcRefs) {
    
  if(ecmcRefs) {
    ecmcPluginDataRefs* dataFromEcmc = (ecmcPluginDataRefs*)ecmcRefs;    
    return dataFromEcmc->ecmcAsynPort;
  }
  return NULL;
}

// register a dummy asyn parameter "plugin.adv.counter"
int initAsyn(void* asynPort) {
  
  ecmcAsynPortDriver *ecmcAsynPort = (ecmcAsynPortDriver*)asynPort;
  if(!ecmcAsynPort) {
    printf("Error: ecmcPlugin_Advanced: ecmcAsynPortDriver NULL.");
    return ECMC_ERROR_ASYNPORT_NULL;
  }

  // Add a dummy counter that incraeses one for each rt cycle
  paramCount = ecmcAsynPort->addNewAvailParam(
                                        "plugin.adv.counter",
                                         asynParamInt32,
                                         (uint8_t *)&(counter),
                                         sizeof(counter),
                                         ECMC_EC_S32,
                                         0);
  if(!paramCount) {
    printf("Error: ecmcPlugin_Advanced: Failed to create asyn param \"plugin.adv.counter\".");
    return ECMC_ERROR_ASYN_PARAM_FAIL;
  }
  paramCount->addSupportedAsynType(asynParamInt32);
  paramCount->allowWriteToEcmc(false);
  paramCount->refreshParam(1);
  return 0;
}

// increase value of counter and refresh asyn param
void increaseCounter(){
  counter++;
  if(paramCount){
    paramCount->refreshParamRT(0);
  }
}
