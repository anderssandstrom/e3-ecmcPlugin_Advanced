/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcAdvanced.h
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*
\*************************************************************************/
#ifndef ECMC_ADVANCED_H_
#define ECMC_ADVANCED_H_

#ifdef __cplusplus
extern "C" {
#endif  // ifdef __cplusplus

double getSampleRate(void* ecmcRefs);
void*  getAsynPort(void* ecmcRefs);

#ifdef __cplusplus
}
#endif  // ifdef __cplusplus

#endif  /* ECMC_ADVANCED_H_ */
