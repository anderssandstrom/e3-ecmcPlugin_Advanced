e3-ecmcPlugin_Advanced
======
ESS Site-specific EPICS module : ecmcPlugin_Advanced

Example illustrating a plugin for use with ecmc.
Shows use of callbacks and implementation, custom ecmc plc-functions and how to access the
ecmcAsynPort object.

# Interface
The interface is defined in the structure ecmcPluginData in ecmcPluginDefs.h:
```
struct ecmcPluginData {
  // Name 
  const char *name;
  // Plugin version
  int version;
  // ECMC_PLUG_VERSION_MAGIC
  int ifVersion;
  // Optional construct func, called once at load
  int (*constructFnc)(void);
  // Optional destruct func, called once at unload
  void (*destructFnc)(void);
  // Optional func that will be called once just before enter realtime mode
  int (*realtimeEnterFnc)(void*);
  // Optional func that will be called once just before exit realtime mode
  int (*realtimeExitFnc)(void);
  // Optional func that will be called each realtime cycle
  int (*realtimeFnc)(int);
  // Allow max ECMC_PLUGIN_MAX_FUNC_COUNT custom plc functions
  struct ecmcOnePlcFunc funcs[ECMC_PLUGIN_MAX_PLC_FUNC_COUNT];
};
```
## Callbacks:

### int  constructFnc(), optional
This callback is called once when the plugin is loaded into ecmc. This is a good place to put code for any initialization needed in the plugin module.
If not used then set "ecmcPluginData.constructFnc=NULL".

Return value: 0 for success or error code.

### void destructFnc(), optional
This callback is called once when the plugin is unloaded. This is a good place to put cleanup code needed by the plugin module.
If not used then set "ecmcPluginData.destructFnc=NULL".

### int realtimeFnc(int ecmcErrorId), optional
This callback is called once in each realtime loop (sync to ecmc). This is a good place to put any cyclic processing needed by the plugin module.
If not used then set "ecmcPluginData.realtimeFnc=NULL".

Parameters: ecmcErrorId: reflects the current errorstate of ecmc.

Return value: 0 for success or error code.

### int realtimeEnterFnc(void* ecmcRefs), optional
This callback is called once just before ecmc enters realtime mode (starts rt-thread). This is a good place to make any prepartions needed before cyclic processing starts.
If not used then set "ecmcPluginData.enterRealTimeFnc=NULL".

Parameters: ecmcRefs: ref to ecmcdata that can be cast to ecmcPluginDataRefs
```
struct ecmcPluginDataRefs {
  double sampleTimeMS;
  ecmcAsynPortDriver *ecmcAsynPort;
};
```
IMPORTANT! This structure is only valid the time between calls of "realtimeEnterFnc()" and "realtimeExitFnc()".

Return value: 0 for success or error code.

### int realtimeExitFnc(), optional
This callback is called once just before ecmc exits realtime mode (exits rt-thread).
If not used then set "ecmcPluginData.exitRealTimeFnc=NULL".

Return value: 0 for success or error code.

### Example:
```
struct ecmcPluginData pluginDataDef = {
  // Name 
  .name = "ecmcExamplePlugin",
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
  // Optional construct func, called once at load. NULL if not definded.
  .constructFnc = exampleConstruct,
  // Optional destruct func, called once at unload. NULL if not definded.
  .destructFnc = exampleDestruct,
  // Optional func that will be called each rt cycle. NULL if not definded.
  .realtimeFnc = exampleRealtime,
  // Optional func that will be called once just before enter realtime mode
  .realtimeEnterFnc = exampleEnterRT,
  // Optional func that will be called once just before exit realtime mode
  .realtimeExitFnc = exampleExitRT,
...
...

```
## PLC functions:
Custom ecmc PLC-functions can be implemnted in plugins. Currentlly the interface supports implementation of up to 32 plc functions. Each plc function needs to be defined by the struct "ecmcOnePlcFunc":
```
struct ecmcOnePlcFunc {
  // Function name (this is the name you use in ecmc plc-code)
  const char *funcName;
  // Number of arguments in the function prototytpe
  int argCount;
  /**
   * 7 different prototypes allowed (only doubles since reg in plc).
   * Only funcArg${argCount} func shall be assigned the rest set to NULL
   * funcArg${argCount}. These need to match. 
   **/
  double (*funcArg0)();
  double (*funcArg1)(double);
  double (*funcArg2)(double,double);
  double (*funcArg3)(double,double,double);
  double (*funcArg4)(double,double,double,double);
  double (*funcArg5)(double,double,double,double,double);
  double (*funcArg6)(double,double,double,double,double,double);
};

```
Example:
```
.funcs[0] =      
      { /*----customPlcFunc1----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "ex_plugin_func_1",
        // Number of arguments in the function prototytpe
        .argCount = 2,
        /**
        * 7 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        * funcArg${argCount}. These need to match. 
        **/
        .funcArg0 = NULL,
        .funcArg1 = NULL,
        .funcArg2 = customPlcFunc1, // Func 1 has 2 args
        .funcArg3 = NULL,
        .funcArg4 = NULL,
        .funcArg6 = NULL,
        .funcArg6 = NULL
      },
  
```
Note: Only the funcArg${argCount} pointer will be used, so set the rest to NULL.

## Example: Module added asynparam linked to record
```
camonitor IOC_TEST:Plugin-Adv-Counter
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:21.535547 23  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:21.630954 24  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:21.740799 25  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:21.838110 26  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:21.930354 27  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.034919 28  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.130358 29  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.234041 30  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.334133 31  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.434937 32  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.530212 33  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.634437 34  
IOC_TEST:Plugin-Adv-Counter    2020-03-26 15:45:22.734606 35  
```
## Dependencies:

### Simple plugins 

Only the "ecmcPluginDefs.h" header is needed.

### Advanced plugins 
When using the "void* ecmcRefs" param (cast to ecmcPluginDataRefs)in the "realtimeEnterFnc()" these additional headers are needed:
* from ecmc:
  * ecmcAsynPortDriver.h
  * ecmcAsynDataItem.h
  * ecmcAsynPortDriverUtils.h
  * ecmcDefinitions.h
  * ecmcErrorsList.h
  * ecmcPluginDataRefs.h
* from asyn:
  * asynPortDriver.h
  
Note: This define is needed in the plugin sources:
```
#define ECMC_IS_PLUGIN
```
