#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL -1
#endif

/**
 * Debug macro
 * Params:
 *   @param level - level of details of debug message.
 *   @param format - Formatting string for the debug message
 *   @param ... -
 */
#define debug(level,format,...) {if(level<=DEBUG_LEVEL){printf( "%s:%i:"#format"\n",__FILE__,__LINE__,##__VA_ARGS__);}}

#endif /* DEBUG_H_ */
