#ifndef __CONFIGUTILS_H
#define __CONFIGUTILS_H

#define CONFIG_START int config() {
#define CONFIG_END return 0; } \
   int __config_ret_hook = config();

#define TO_STR(x) #x
#define STR(x) TO_STR(x)

#define START (
#define END );

#endif // __CONFIGUTILS_H